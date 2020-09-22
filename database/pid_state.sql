CREATE TABLE pid_state (
  time timestamp  primary key NOT NULL,
  oven_temp float DEFAULT NULL,
  avg_oven_temp float DEFAULT NULL,
  output float DEFAULT NULL,
  p_term float DEFAULT NULL,
  d_term float DEFAULT NULL,
  i_term float DEFAULT NULL,
  error_sum float DEFAULT NULL,
  opt_probe1_label varchar(45) DEFAULT NULL,
  opt_probe1_temp float DEFAULT NULL,
  opt_probe2_label varchar(45) DEFAULT NULL,
  opt_probe2_temp float DEFAULT NULL,
  opt_probe3_label varchar(45) DEFAULT NULL,
  opt_probe3_temp float DEFAULT NULL,
  pid_config char(36) DEFAULT NULL,
  set_point int DEFAULT NULL,
  local_time bigint unique DEFAULT NULL
)