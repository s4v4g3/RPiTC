var express = require('express');
var router = express.Router();
var mysql = require('mysql')
const {
    performance,
  } = require('perf_hooks');

var pool = mysql.createPool({
    connectionLimit: 100,
    host: "localhost",
    user: "bbq",
    password: "bbq",
    database: "bbq"
});


/* GET data page. */
router.get('/', function(req, res, next) {
    let secs = 3600
    if (req.query.sec) {
        secs = parseInt(req.query.sec)
    }
    console.log(`getting ${secs} secs of data from database `)
    var t0 = performance.now();

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
});

module.exports = router;