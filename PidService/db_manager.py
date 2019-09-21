import sqlalchemy as db
from data_logger import LoggerMgr, ConsoleColor
import datetime
import hashlib
import json
from dateutil import tz

__all__ = ["DbManager"]

class DbManager(object):

    @staticmethod
    def create_db_engine():
        return AlchemyEngine()



class AlchemyEngine(object):

    def __init__(self):
        self.engine = db.create_engine('mysql+mysqlconnector://bbq:bbq@localhost/bbq')
        self.connection = self.engine.connect()
        metadata = db.MetaData()
        self.pid_state_table = db.Table('pid_state', metadata, autoload=True, autoload_with=self.engine)
        metadata = db.MetaData()
        self.pid_config_table = db.Table('pid_config', metadata, autoload=True, autoload_with=self.engine)
        """"""

    def insert(self, pid_config, pid_state):

        # compute hash of pid_config
        config_hash = hashlib.md5(json.dumps(pid_config.as_dict(), sort_keys=True).encode()).hexdigest()

        s = db.select([self.pid_config_table.c.id]).where(self.pid_config_table.columns.id == config_hash)
        result = self.connection.execute(s)
        count = result.rowcount
        if count == 0:
            LoggerMgr.info("Inserting new pid_config row with id={}".format(config_hash), color=ConsoleColor.RED)
            query = db.insert(self.pid_config_table).values(
                id=config_hash,
                set_point=pid_config.set_point,
                kb=pid_config.kb,
                kp=pid_config.kp,
                ki=pid_config.ki,
                kd=pid_config.kd,
                smooth_coeff=pid_config.smooth_coeff,
                max_output=pid_config.max_output
            )
            result = self.connection.execute(query)

        now = datetime.datetime.now()
        from_zone = tz.tzutc()
        to_zone = tz.tzlocal()
        utc = now.replace(tzinfo=from_zone)
        local_time = utc.astimezone(to_zone)

        query = db.insert(self.pid_state_table).values(
            time=now,
            oven_temp=pid_state.last_oven_temp,
            avg_oven_temp=pid_state.avg_oven_temp,
            output=pid_state.output,
            p_term=pid_state.p_term,
            d_term=pid_state.d_term,
            i_term=pid_state.i_term,
            error_sum=pid_state.error_sum,
            set_point=pid_config.set_point,
            pid_config=config_hash,
            local_time=local_time.timestamp())
        result = self.connection.execute(query)


