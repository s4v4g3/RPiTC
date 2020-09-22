CREATE TABLE pid_config (
  id char(36) primary key NOT NULL,
  set_point int DEFAULT NULL,
  kb float DEFAULT NULL,
  kp float DEFAULT NULL,
  ki float DEFAULT NULL,
  kd float DEFAULT NULL,
  smooth_coeff float DEFAULT NULL,
  max_output float DEFAULT NULL
)
