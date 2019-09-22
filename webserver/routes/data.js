var express = require('express');
var router = express.Router();
var mysql = require('mysql')

var conn = mysql.createConnection({
    host: "localhost",
    user: "bbq",
    password: "bbq",
    database: "bbq"
});


/* GET data page. */
router.get('/', function(req, res, next) {
    secs = req.query.sec
  conn.connect(function(err) {
      if (err){
          res.status(200).json({})
      }
      else {
        let date = Math.floor(new Date() / 1000)
        let tzOffset = (new Date()).getTimezoneOffset() * 60
        let localTime = (date - tzOffset)
        let startDate = (localTime) - secs
        let select = `SELECT * from pid_view where local_time > ${startDate} ORDER BY time asc`
        console.log(select)
        conn.query(select, function (err, result, fields){
            data = result.map(a => {})
            res.status(200).json(result)
            })
      }
    });
});

module.exports = router;