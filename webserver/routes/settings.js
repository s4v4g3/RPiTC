var express = require('express');
var router = express.Router();
var mysql = require('mysql')
var zmq = require('zeromq')

var hostname = "10.4.1.240"

var pool = mysql.createPool({
    connectionLimit: 100,
    host: hostname,
    user: "bbq",
    password: "bbq",
    database: "bbq"
});


/* GET settings page. */
/*
router.get('/', function(req, res, next) {
    let secs = 3600
    if (req.query.sec) {
        secs = parseInt(req.query.sec)
    }
    console.log(`secs = ${secs}`)
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
                    data = result.map(a => {})
                    res.status(200).json(result)
                }
            });
            connection.release()
        }       
    });
});
*/

router.post('/', function(req, res) {
    let body = req.body;
    var zmqsock = zmq.socket('req');
    zmqsock.connect(`tcp://${hostname}:12355`)
    zmqsock.on("message", function(reply) {
        data = JSON.parse(reply)
        console.log(`zmq response: ${data}`)
        zmqsock.close();
        res.status(200).json({reply: data})
    });
    zmq_message = JSON.stringify(body)
    console.log(`zmq request: ${zmq_message}`)
    zmqsock.send(zmq_message)

});

module.exports = router;