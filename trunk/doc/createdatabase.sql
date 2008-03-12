CREATE TABLE users
(
  user_id varchar(20) NOT NULL,
  email varchar(100),
  show_email char(1) DEFAULT 'Y',
  submits int(11) DEFAULT 0,
  solveds int(11) DEFAULT 0,
  available char(1) DEFAULT 'N',
  last_login_ip varchar(20),
  last_login_time datetime,
  volume int(11),
  language tinyint(4),
  password text NOT NULL,
  reg_time datetime NOT NULL,
  nickname varchar(100),
  school varchar(100),
  permission char(6) DEFAULT '------',
  share_code char(1) DEFAULT 'N',
  indentify_code char(16),
  PRIMARY KEY(user_id)
)

CREATE TABLE problems
(
  problem_id int(11) NOT NULL AUTO_INCREMENT,
  title varchar(200) NOT NULL,
  description text NOT NULL,
  input text NOT NULL,
  output text NOT NULL,
  sample_input text NOT NULL,
  sample_output text NOT NULL,
  hint text,
  source varchar(100),
  addin_time datetime NOT NULL,
  time_limit int(11) NOT NULL,
  case_time_limit int(11) NOT NULL,
  memory_limit int(11) NOT NULL,
  available char(1) DEFAULT 'N',
  accepted int(11) DEFAULT 0,
  submit int(11) DEFAULT 0,
  solved_users int(11) DEFAULT 0,
  submit_users int(11) DEFAULT 0,
  standard_time_limit int(11),
  standard_memory_limit int(11),
  PRIMARY KEY(problem_id) 
)

CREATE TABLE contests
(
  contest_id int(11) NOT NULL AUTO_INCREMENT,
  public_id int(11) DEFAULT 0,
  title varchar(200) NOT NULL,
  start_time datetime NOT NULL,
  end_time datetime NOT NULL,
  contest_type char(1) DEFAULT 'N',
  description text,
  PRIMARY KEY(contest_id)
)

CREATE TABLE mails
(
  mail_id int(11) NOT NULL AUTO_INCREMENT,
  topic_id int(11) NOT NULL,
  title varchar(200) NOT NULL,
  content text,
  unread char(1) DEFAULT 'Y',
  time datetime NOT NULL,
  to_user varchar(20) NOT NULL,
  from_user varchar(20) NOT NULL,
  PRIMARY KEY(mail_id),
  FOREIGN KEY(to_user) REFERENCES users,
  FOREIGN KEY(from_user) REFERENCES users,
  FOREIGN KEY(topic_id) REFERENCES mails
)

CREATE TABLE discusses
(
  message_id int(11) NOT NULL AUTO_INCREMENT ,
  reply_id int(11) NOT NULL,
  topic_id int(11) NOT NULL,
  user_id varchar(20) NOT NULL,
  problem_id int(11) DEFAULT 0,
  contest_id int(11) DEFAULT 0,
  title varchar(200),
  content text,
  time datetime,
  PRIMARY KEY(message_id),
  FOREIGN KEY(user_id) REFERENCES users,
  FOREIGN KEY(problem_id) REFERENCES problems,
  FOREIGN KEY(contest_id) REFERENCES contests,
  FOREIGN KEY(reply_id) REFERENCES dicusses,
  FOREIGN KEY(topic_id) REFERENCES dicusses
)

CREATE TABLE files
(
  file_id int(11) NOT NULL AUTO_INCREMENT,
  path text NOT NULL,
  style tinyint(4) DEFAULT 0,
  PRIMARY KEY(file_id)
)

CREATE TABLE errors
(
  error_id int(11) NOT NULL AUTO_INCREMENT,
  content text NOT NULL,
  PRIMARY KEY(error_id)
)

CREATE TABLE codes
(
  code_id int(11) NOT NULL AUTO_INCREMENT,
  share char(1) DEFAULT 'Y',
  code_content text,
  PRIMARY KEY(code_id)
)

CREATE TABLE news
(
  news_id int(11) NOT NULL AUTO_INCREMENT,
  publishtime datetime NOT NULL,
  title varchar(200) NOT NULL,
  content text,
  PRIMARY KEY(news_id)
)

CREATE TABLE statuses
(
  status_id int(11) NOT NULL AUTO_INCREMENT,
  user_id varchar(20) NOT NULL,
  problem_id int(11) NOT NULL,
  contest_id int(11) DEFAULT 0,
  time int(11) DEFAULT 0,
  memory int(11) DEFAULT 0,
  submit_time datetime NOT NULL,
  result tinyint(4) DEFAULT 15,
  language tinyint(4) NOT NULL,
  code_id int(11) NOT NULL,
  code_length int(11) NOT NULL,
  submit_ip varchar(20) NOT NULL,
  error_id int(11) DEFAULT 0,
  PRIMARY KEY(status_id),
  FOREIGN KEY(user_id) REFERENCES users,
  FOREIGN KEY(problem_id) REFERENCES problems,
  FOREIGN KEY(contest_id) REFERENCES contests,
  FOREIGN KEY(code_id) REFERENCES codes,
  FOREIGN KEY(error_id) REFERENCES errors 
)

CREATE TABLE contestpermission
(
  user_id varchar(20) NOT NULL,
  contest_id int(11) NOT NULL,
  PRIMARY KEY(user_id,contest_id),
  FOREIGN KEY(user_id) REFERENCES users,
  FOREIGN KEY(contest_id) REFERENCES contests
)

CREATE TABLE problemtocontests
(
  problem_id int(11) NOT NULL,
  contest_id int(11) NOT NULL,
  in_contest_id int(11) NOT NULL,
  PRIMARY KEY(problem_id,contest_id),
  FOREIGN KEY(problem_id) REFERENCES problems,
  FOREIGN KEY(contest_id) REFERENCES contests
)

CREATE TABLE filestoproblems
(
  file_id int(11) NOT NULL,
  problem_id int(11) NOT NULL,
  PRIMARY KEY(file_id,problem_id),
  FOREIGN KEY(file_id) REFERENCES users,
  FOREIGN KEY(problem_id) REFERENCES contests
)

CREATE TABLE filestocontests
(
  file_id int(11) NOT NULL,
  contest_id int(11) NOT NULL,
  PRIMARY KEY(file_id,contest_id),
  FOREIGN KEY(file_id) REFERENCES users,
  FOREIGN KEY(contest_id) REFERENCES contests
)
