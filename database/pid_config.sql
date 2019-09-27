CREATE TABLE `pid_config` (
  `id` char(36) NOT NULL,
  `set_point` int(11) DEFAULT NULL,
  `kb` float DEFAULT NULL,
  `kp` float DEFAULT NULL,
  `ki` float DEFAULT NULL,
  `kd` float DEFAULT NULL,
  `smooth_coeff` float DEFAULT NULL,
  `max_output` float DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='			';
