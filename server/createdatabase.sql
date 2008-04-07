-- MySQL dump 10.11
--
-- Host: localhost    Database: onlinejudgetest
-- ------------------------------------------------------
-- Server version	5.0.45-Debian_1ubuntu3.3-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `codes`
--

DROP TABLE IF EXISTS `codes`;
CREATE TABLE `codes` (
  `code_id` int(11) NOT NULL auto_increment,
  `share` char(1) default 'Y',
  `code_content` text,
  PRIMARY KEY  (`code_id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `codes`
--

LOCK TABLES `codes` WRITE;
/*!40000 ALTER TABLE `codes` DISABLE KEYS */;
INSERT INTO `codes` VALUES (1,'Y','printf(\"\\%d\",a.getchar())');
/*!40000 ALTER TABLE `codes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `contestpermission`
--

DROP TABLE IF EXISTS `contestpermission`;
CREATE TABLE `contestpermission` (
  `user_id` varchar(20) NOT NULL,
  `contest_id` int(11) NOT NULL,
  PRIMARY KEY  (`user_id`,`contest_id`),
  KEY `contest_id` (`contest_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `contestpermission`
--

LOCK TABLES `contestpermission` WRITE;
/*!40000 ALTER TABLE `contestpermission` DISABLE KEYS */;
INSERT INTO `contestpermission` VALUES ('Apai',1),('LiuQian',1),('macheng',1);
/*!40000 ALTER TABLE `contestpermission` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `contests`
--

DROP TABLE IF EXISTS `contests`;
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
) ENGINE=MyISAM AUTO_INCREMENT=8 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `contests`
--

LOCK TABLES `contests` WRITE;
/*!40000 ALTER TABLE `contests` DISABLE KEYS */;
INSERT INTO `contests` VALUES (1,1,'Apai\'s Apple','2008-03-20 10:00:00','2008-03-20 16:00:00','N','Apai want to eat a lot of apples.',1,'Y'),(2,1,'test contest','2008-03-21 10:00:00','2008-03-21 16:00:00','N','abcdefg',1,'Y'),(3,1,'test contest','2008-03-12 09:10:00','2008-03-12 10:00:00','N','abcdefg',1,'Y'),(4,1,'test contest','2008-03-12 09:10:00','2008-03-12 10:00:00','N','abcdefg',1,'Y'),(5,1,'test contest','2008-03-12 09:10:00','2008-03-12 10:00:00','N','abcdefg',1,'Y'),(6,1,'test contest','2008-03-12 09:10:00','2008-03-12 10:00:00','N','abcdefg',1,'Y'),(7,1,'test contest','2008-03-12 09:10:00','2008-03-12 10:00:00','N','abcdefg',1,'Y');
/*!40000 ALTER TABLE `contests` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `discusses`
--

DROP TABLE IF EXISTS `discusses`;
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
) ENGINE=MyISAM AUTO_INCREMENT=7 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `discusses`
--

LOCK TABLES `discusses` WRITE;
/*!40000 ALTER TABLE `discusses` DISABLE KEYS */;
INSERT INTO `discusses` VALUES (2,0,2,'Apai',1,1,'bcdApai\'sa','Today we will to chi shaokao.','2004-03-14 10:00:00','Y'),(3,0,3,'Apai',1,1,'sdadfa','Today we will to chi shaokao.','0000-00-00 00:00:00','Y'),(4,0,4,'Apai',1,1,'Apai\'s','Today we will to chi shaokao.','2004-03-14 10:00:00','Y'),(5,0,5,'Apai',1,1,'Apai\'s','Today we will to chi shaokao.','2004-03-14 10:00:00','N'),(6,0,6,'Apai',1,3,'Apai\'s','Today we will to chi shaokao.','2004-03-14 10:00:00','Y');
/*!40000 ALTER TABLE `discusses` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `errors`
--

DROP TABLE IF EXISTS `errors`;
CREATE TABLE `errors` (
  `error_id` int(11) NOT NULL auto_increment,
  `content` text NOT NULL,
  PRIMARY KEY  (`error_id`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `errors`
--

LOCK TABLES `errors` WRITE;
/*!40000 ALTER TABLE `errors` DISABLE KEYS */;
INSERT INTO `errors` VALUES (2,'Error on macheng');
/*!40000 ALTER TABLE `errors` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `files`
--

DROP TABLE IF EXISTS `files`;
CREATE TABLE `files` (
  `file_id` int(11) NOT NULL auto_increment,
  `path` text NOT NULL,
  `style` tinyint(4) default '0',
  `disable` char(1) default 'N',
  PRIMARY KEY  (`file_id`)
) ENGINE=MyISAM AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `files`
--

LOCK TABLES `files` WRITE;
/*!40000 ALTER TABLE `files` DISABLE KEYS */;
INSERT INTO `files` VALUES (1,'/usr/abc/a/in',1,'N'),(2,'/usr/abc/a/in',1,'N'),(3,'/user/a/abc/out',1,'N'),(4,'/user/inc/abc/out',1,'N');
/*!40000 ALTER TABLE `files` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `filestocontests`
--

DROP TABLE IF EXISTS `filestocontests`;
CREATE TABLE `filestocontests` (
  `file_id` int(11) NOT NULL,
  `contest_id` int(11) NOT NULL,
  `version` int(11) default '1',
  PRIMARY KEY  (`file_id`,`contest_id`),
  KEY `contest_id` (`contest_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `filestocontests`
--

LOCK TABLES `filestocontests` WRITE;
/*!40000 ALTER TABLE `filestocontests` DISABLE KEYS */;
INSERT INTO `filestocontests` VALUES (1,1,1);
/*!40000 ALTER TABLE `filestocontests` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `filestoproblems`
--

DROP TABLE IF EXISTS `filestoproblems`;
CREATE TABLE `filestoproblems` (
  `file_id` int(11) NOT NULL,
  `problem_id` int(11) NOT NULL,
  `version` int(11) default '1',
  PRIMARY KEY  (`file_id`,`problem_id`),
  KEY `problem_id` (`problem_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `filestoproblems`
--

LOCK TABLES `filestoproblems` WRITE;
/*!40000 ALTER TABLE `filestoproblems` DISABLE KEYS */;
INSERT INTO `filestoproblems` VALUES (1,1,1),(2,1,2),(4,3,1);
/*!40000 ALTER TABLE `filestoproblems` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `intooutfiles`
--

DROP TABLE IF EXISTS `intooutfiles`;
CREATE TABLE `intooutfiles` (
  `in_id` int(11) NOT NULL default '0',
  `out_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`in_id`,`out_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `intooutfiles`
--

LOCK TABLES `intooutfiles` WRITE;
/*!40000 ALTER TABLE `intooutfiles` DISABLE KEYS */;
/*!40000 ALTER TABLE `intooutfiles` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mails`
--

DROP TABLE IF EXISTS `mails`;
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
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `mails`
--

LOCK TABLES `mails` WRITE;
/*!40000 ALTER TABLE `mails` DISABLE KEYS */;
INSERT INTO `mails` VALUES (1,0,'fuck','The fuck','N','2008-03-13 10:00:00','aPai','ooeyusea','N','N'),(2,2,'fuck','The fuck','N','2008-03-13 10:00:00','aPai','ooeyusea','N','N'),(3,3,'fu','the fuck','N','2008-03-15 10:00:00','ooeyuse','aPai','N','N');
/*!40000 ALTER TABLE `mails` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `news`
--

DROP TABLE IF EXISTS `news`;
CREATE TABLE `news` (
  `news_id` int(11) NOT NULL auto_increment,
  `publishtime` datetime NOT NULL,
  `title` varchar(200) NOT NULL,
  `content` text,
  PRIMARY KEY  (`news_id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `news`
--

LOCK TABLES `news` WRITE;
/*!40000 ALTER TABLE `news` DISABLE KEYS */;
INSERT INTO `news` VALUES (1,'2008-03-13 10:00:00','Apai say.','Let\'s fuck.');
/*!40000 ALTER TABLE `news` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `problems`
--

DROP TABLE IF EXISTS `problems`;
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
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `problems`
--

LOCK TABLES `problems` WRITE;
/*!40000 ALTER TABLE `problems` DISABLE KEYS */;
INSERT INTO `problems` VALUES (1,'Apai\'s Thinkpad','Apai\'s Thinkpad xxxxx','XXX','OOO','XXXX','OOOO','','','2008-03-13 10:00:00',12,2,64,'N',1,0,0,0,0,12,1,'N'),(2,'abcdefg','Adaf','XXX','OOO','XXXX','OOOO','','','2008-03-13 10:00:00',12,2,64,'Y',1,0,0,0,0,12,1,'N');
/*!40000 ALTER TABLE `problems` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `problemtocontests`
--

DROP TABLE IF EXISTS `problemtocontests`;
CREATE TABLE `problemtocontests` (
  `problem_id` int(11) NOT NULL,
  `contest_id` int(11) NOT NULL,
  `in_contest_id` int(11) NOT NULL,
  PRIMARY KEY  (`problem_id`,`contest_id`),
  KEY `contest_id` (`contest_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `problemtocontests`
--

LOCK TABLES `problemtocontests` WRITE;
/*!40000 ALTER TABLE `problemtocontests` DISABLE KEYS */;
INSERT INTO `problemtocontests` VALUES (4,1,3),(2,1,2),(1,1,1);
/*!40000 ALTER TABLE `problemtocontests` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `statuses`
--

DROP TABLE IF EXISTS `statuses`;
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
  PRIMARY KEY  (`status_id`),
  KEY `user_id` (`user_id`),
  KEY `problem_id` (`problem_id`),
  KEY `contest_id` (`contest_id`),
  KEY `code_id` (`code_id`),
  KEY `error_id` (`error_id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `statuses`
--

LOCK TABLES `statuses` WRITE;
/*!40000 ALTER TABLE `statuses` DISABLE KEYS */;
INSERT INTO `statuses` VALUES (1,'macheng',1,0,4,10,'2008-03-13 10:00:00',0,0,1,0,'192.168.5.123',0);
/*!40000 ALTER TABLE `statuses` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
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
  `password` text NOT NULL,
  `reg_time` datetime NOT NULL,
  `nickname` varchar(100) default NULL,
  `school` varchar(100) default NULL,
  `permission` char(6) default '------',
  `share_code` char(1) default 'N',
  `indentify_code` char(16) default NULL,
  PRIMARY KEY  (`user_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `users`
--

LOCK TABLES `users` WRITE;
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` VALUES ('',NULL,'Y',3,4,'N',NULL,NULL,NULL,NULL,'12345','2008-03-21 10:00:00',NULL,NULL,'------','N',NULL),('macheng','a@a.com','Y',3,4,'N','','0000-00-00 00:00:00',1,0,'654321','2008-03-14 10:00:00','machengabcdefg','whu','------','N','');
/*!40000 ALTER TABLE `users` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2008-04-02  8:47:46
