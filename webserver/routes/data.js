var express = require('express');
var router = express.Router();
var mysql = require('mysql')
const { Pool, Client } = require('pg')
const {
    performance,
  } = require('perf_hooks');

var pool = null;
const useMySql = false;

let host = process.env.DB_HOST || 'pibbq.savage.zone'
let user = process.env.DB_USER || 'bbq'
let password = process.env.DB_PASSWORD || 'bbq'
let database = process.env.DB_DBNAME || 'bbq'
let port = process.env.DB_PORT || 5432

function initDb() {

    if (pool === null) {
        if (useMySql) {
            pool = mysql.createPool({
                connectionLimit: 100,
                host: "localhost",
                user: "bbq",
                password: "bbq",
                database: "bbq"
            });
        } else {
            pool = new Pool({
                user,
                host,
                database,
                password,
                port
            })
        }
    }
}


function queryData(select, callback) {
    initDb();
    if (useMySql) {
        pool.getConnection(function(err, connection) {
            if (err){
                callback(err)
            }
            else {
                console.log(select)
                connection.query(select, function (err, result){
                    callback(err, result)
                });
                connection.release()
            }
        });
    } else {
        pool.connect((err, client, done) => {
            if (err) {
                callback(err)
            }
            else {
                client.query(select, function (err, result){
                    callback(err, result.rows)
                    done()
                })
            }
        })
    }
}


/* GET data page. */
router.get('/', function(req, res, next) {
    let secs = 3600
    if (req.query.sec) {
        secs = parseInt(req.query.sec)
    }
    console.log(`getting ${secs} secs of data from database `)
    var t0 = performance.now();

    let date = Math.floor(new Date() / 1000)
    let tzOffset = (new Date()).getTimezoneOffset() * 60
    let localTime = (date - tzOffset)
    let startDate = (localTime) - secs
    let select = `SELECT * from pid_view where local_time > ${startDate} ORDER BY time asc`
    console.log(select)

    queryData(select, function (err, result, fields) {
        if (err){
            console.log(err)
            res.status(200).json({error: err})
        } else {
            var t1 = performance.now();
            console.log("database query took " + (t1 - t0) + " milliseconds.");
            res.status(200).json(result)
        }
    })

    /*
    pool.getConnection(function(err, connection) {
        if (err){
            console.log(err)
            res.status(200).json({error: err})
        }
        else {
            let date = Math.floor(new Date() / 1000)
            let tzOffset = (new Date()).getTimezoneOffset() * 60
            let localTime = (date - tzOffset)
            let startDate = (localTime) - secs
            let select = `SELECT * from pid_view where local_time > ${startDate} ORDER BY time asc`
            console.log(select)
            connection.query(select, function (err, result, fields){
                if (err) {
                    console.log(err);
                    res.status(200).json({error: err})
                }
                else {
                    //data = result.map(a => {})
                    var t1 = performance.now();
                    console.log("database query took " + (t1 - t0) + " milliseconds.");
                    res.status(200).json(result)
                }
            });
            connection.release()
        }       
    });
    */
});

router.get('/latest', function(req, res, next) {
    
})

module.exports = router;