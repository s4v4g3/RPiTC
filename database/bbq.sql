CREATE DATABASE `bbq` /*!40100 DEFAULT CHARACTER SET utf8mb4 */;

CREATE USER 'bbq'@'localhost' IDENTIFIED BY 'bbq';

GRANT ALL on bbq.* TO 'bbq'@'localhost';