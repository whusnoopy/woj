CREATE TABLE `codes` (
  `code_id` int(11) NOT NULL auto_increment,
  `share` char(1) default 'Y',
  `code_content` text,
  PRIMARY KEY  (`code_id`)
) ENGINE=MyISAM;

CREATE TABLE `contestpermission` (
  `user_id` varchar(20) NOT NULL,
  `contest_id` int(11) NOT NULL,
  PRIMARY KEY  (`user_id`,`contest_id`),
  KEY `contest_id` (`contest_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `contests` (
  `contest_id` int(11) NOT NULL auto_increment,
  `public_id` int(11) default '0',
  `title` varchar(200) NOT NULL,
  `start_time` datetime NOT NULL,
  `end_time` datetime NOT NULL,
  `contest_type` char(1) default 'N',
  `description` text,
  `version` int(11) default '1',
  `available` char(1) default 'Y',
  PRIMARY KEY  (`contest_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `courses` (
  `course_id` int(11) NOT NULL auto_increment,
  `description` text,
  `available` char(1) default 'Y',
  `user_id` varchar(20) NOT NULL,
  PRIMARY KEY  (`course_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `discusses` (
  `message_id` int(11) NOT NULL auto_increment,
  `reply_id` int(11) NOT NULL,
  `topic_id` int(11) NOT NULL,
  `user_id` varchar(20) NOT NULL,
  `problem_id` int(11) default '0',
  `contest_id` int(11) default '0',
  `title` varchar(200) default NULL,
  `content` text,
  `time` datetime NOT NULL,
  `available` char(1) default 'Y',
  PRIMARY KEY  (`message_id`),
  KEY `user_id` (`user_id`),
  KEY `problem_id` (`problem_id`),
  KEY `contest_id` (`contest_id`),
  KEY `reply_id` (`reply_id`),
  KEY `topic_id` (`topic_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `errors` (
  `error_id` int(11) NOT NULL auto_increment,
  `content` text NOT NULL,
  PRIMARY KEY  (`error_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `files` (
  `file_id` int(11) NOT NULL auto_increment,
  `path` text NOT NULL,
  `style` tinyint(4) default '0',
  `disable` char(1) default 'N',
  PRIMARY KEY  (`file_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `filestocontests` (
  `file_id` int(11) NOT NULL,
  `contest_id` int(11) NOT NULL,
  `version` int(11) default '1',
  PRIMARY KEY  (`file_id`,`contest_id`),
  KEY `contest_id` (`contest_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `filestoproblems` (
  `file_id` int(11) NOT NULL,
  `problem_id` int(11) NOT NULL,
  `version` int(11) default '1',
  PRIMARY KEY  (`file_id`,`problem_id`),
  KEY `problem_id` (`problem_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `intooutfiles` (
  `in_id` int(11) NOT NULL default '0',
  `out_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`in_id`,`out_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `jobs` (
  `job_id` int(11) NOT NULL auto_increment,
  `description` text,
  `publish_time` datetime default NULL,
  `year` int(11) NOT NULL,
  `term` char(1) NOT NULL default 'F',
  `available` char(1) default 'Y',
  `course_id` int(11) NOT NULL,
  PRIMARY KEY  (`job_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `mails` (
  `mail_id` int(11) NOT NULL auto_increment,
  `topic_id` int(11) NOT NULL,
  `title` varchar(200) NOT NULL,
  `content` text,
  `unread` char(1) default 'Y',
  `time` datetime NOT NULL,
  `to_user` varchar(20) NOT NULL,
  `from_user` varchar(20) NOT NULL,
  `reader_del` char(1) default 'N',
  `writer_del` char(1) default 'N',
  PRIMARY KEY  (`mail_id`),
  KEY `to_user` (`to_user`),
  KEY `from_user` (`from_user`),
  KEY `topic_id` (`topic_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;

CREATE TABLE `most_diligen_player` (
  `day_user` varchar(20) NOT NULL,
  `week_user` varchar(20) NOT NULL,
  `month_user` varchar(20) NOT NULL
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;

INSERT INTO `most_diligen_player` VALUES ('snoopy', 'magiii', 'ooeyusea');

CREATE TABLE `news` (
  `news_id` int(11) NOT NULL auto_increment,
  `publishtime` datetime NOT NULL,
  `title` varchar(200) NOT NULL,
  `content` text,
  PRIMARY KEY  (`news_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `problems` (
  `problem_id` int(11) NOT NULL auto_increment,
  `title` varchar(200) NOT NULL,
  `description` text NOT NULL,
  `input` text NOT NULL,
  `output` text NOT NULL,
  `sample_input` text NOT NULL,
  `sample_output` text NOT NULL,
  `hint` text,
  `source` varchar(100) default NULL,
  `addin_time` datetime NOT NULL,
  `time_limit` int(11) NOT NULL,
  `case_time_limit` int(11) NOT NULL,
  `memory_limit` int(11) NOT NULL,
  `available` char(1) default 'N',
  `accepted` int(11) default '0',
  `submit` int(11) default '0',
  `solved_users` int(11) default '0',
  `submit_users` int(11) default '0',
  `standard_time_limit` int(11) default NULL,
  `standard_memory_limit` int(11) default NULL,
  `version` int(11) default '0',
  `spj` char(1) default 'N',
  PRIMARY KEY  (`problem_id`)
) ENGINE=MyISAM AUTO_INCREMENT=1001 DEFAULT CHARSET=latin1;

CREATE TABLE `problemstojobs` (
  `job_id` int(11) NOT NULL,
  `problem_id` int(11) NOT NULL,
  PRIMARY KEY  (`job_id`,`problem_id`),
  KEY `problem_id` (`problem_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `problemstosets` (
  `set_id` int(11) NOT NULL,
  `problem_id` int(11) NOT NULL,
  PRIMARY KEY  (`set_id`,`problem_id`),
  KEY `problem_id` (`problem_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `problemtocontests` (
  `problem_id` int(11) NOT NULL,
  `contest_id` int(11) NOT NULL,
  `in_contest_id` int(11) NOT NULL,
  PRIMARY KEY  (`problem_id`,`contest_id`),
  KEY `contest_id` (`contest_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `sets` (
  `set_id` int(11) NOT NULL auto_increment,
  `number` int(11) default '1',
  PRIMARY KEY  (`set_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `setstojobs` (
  `job_id` int(11) NOT NULL,
  `set_id` int(11) NOT NULL,
  PRIMARY KEY  (`job_id`,`set_id`),
  KEY `set_id` (`set_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `statuses` (
  `status_id` int(11) NOT NULL auto_increment,
  `user_id` varchar(20) NOT NULL,
  `problem_id` int(11) NOT NULL,
  `contest_id` int(11) default '0',
  `time` int(11) default '0',
  `memory` int(11) default '0',
  `submit_time` datetime NOT NULL,
  `result` tinyint(4) default '15',
  `language` tinyint(4) NOT NULL,
  `code_id` int(11) NOT NULL,
  `code_length` int(11) NOT NULL,
  `submit_ip` varchar(20) NOT NULL,
  `error_id` int(11) default '0',
  `type` char(1) default 'N',
  PRIMARY KEY  (`status_id`),
  KEY `user_id` (`user_id`),
  KEY `problem_id` (`problem_id`),
  KEY `contest_id` (`contest_id`),
  KEY `code_id` (`code_id`),
  KEY `error_id` (`error_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `students` (
  `user_id` varchar(20) NOT NULL,
  `student_id` varchar(20) NOT NULL,
  `realname` varchar(20) NOT NULL,
  `grade` int(11) NOT NULL,
  `class` int(11) NOT NULL,
  `available` char(1) default 'Y',
  PRIMARY KEY  (`user_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `studentstocourse` (
  `user_id` varchar(20) NOT NULL,
  `course_id` int(11) NOT NULL,
  PRIMARY KEY  (`user_id`,`course_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `teachers` (
  `user_id` varchar(20) NOT NULL,
  `realname` varchar(20) default NULL,
  `available` char(1) default 'Y',
  PRIMARY KEY  (`user_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE `users` (
  `user_id` varchar(20) NOT NULL,
  `email` varchar(100) default NULL,
  `show_email` char(1) default 'Y',
  `submits` int(11) default '0',
  `solveds` int(11) default '0',
  `available` char(1) default 'N',
  `last_login_ip` varchar(20) default NULL,
  `last_login_time` datetime default NULL,
  `volume` int(11) default NULL,
  `language` tinyint(4) default NULL,
  `password` varchar(100) NOT NULL,
  `reg_time` datetime NOT NULL,
  `nickname` varchar(100) default NULL,
  `school` varchar(100) default NULL,
  `permission` char(6) default '------',
  `share_code` char(1) default 'N',
  `indentify_code` char(16) default NULL,
  PRIMARY KEY  (`user_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

