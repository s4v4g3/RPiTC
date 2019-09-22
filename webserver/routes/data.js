var express = require('express');
var router = express.Router();
var mysql = require('mysql')

var conn = mysql.createConnection({
    host: "10.4.1.240",
    user: "bbq",
    password: "bbq",
    database: "bbq"
});


/* GET data page. */
router.get('/', function(req, res, next) {
  conn.connect(function(err) {
    conn.query("SELECT * from pid_state ORDER BY time asc", function (err, result, fields){
        res.status(200).json(result)
    })
  });
});

module.exports = router;