-- phpMyAdmin SQL Dump
-- version 3.4.11.1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Feb 16, 2013 at 08:02 AM
-- Server version: 5.5.30
-- PHP Version: 5.2.17

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `wbreezec_iacreg`
--

-- --------------------------------------------------------

--
-- Table structure for table `contest`
--

DROP TABLE IF EXISTS `contest`;
CREATE TABLE IF NOT EXISTS `contest` (
  `ctstID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `regYear` smallint(5) unsigned NOT NULL,
  `name` varchar(72) NOT NULL,
  `location` varchar(72) DEFAULT NULL,
  `chapter` smallint(5) unsigned DEFAULT NULL,
  `startDate` date NOT NULL,
  `endDate` date NOT NULL,
  `regDeadline` date NOT NULL,
  `homeURL` text NOT NULL,
  `regEmail` text NOT NULL,
  `hasVoteJudge` enum('y','n') NOT NULL DEFAULT 'n',
  `reqPmtForVoteJudge` enum('y','n') NOT NULL DEFAULT 'n',
  `voteEmail` text,
  `hasPayPal` enum('y','n') NOT NULL DEFAULT 'n',
  `payEmail` text,
  `hasPracticeReg` enum('y','n') NOT NULL DEFAULT 'n',
  `reqPmtForPracticeReg` enum('y','n') NOT NULL DEFAULT 'n',
  `maxPracticeSlots` smallint(5) unsigned DEFAULT NULL,
  `regOpen` date NOT NULL,
  `question` varchar(4096) DEFAULT NULL,
  PRIMARY KEY (`ctstID`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=125 ;

-- --------------------------------------------------------

--
-- Table structure for table `ctst_admin`
--

DROP TABLE IF EXISTS `ctst_admin`;
CREATE TABLE IF NOT EXISTS `ctst_admin` (
  `ctstID` int(10) unsigned NOT NULL,
  `userID` int(10) unsigned NOT NULL,
  `roles` set('admin','cd','registrar','vc') DEFAULT NULL,
  KEY `ctstID` (`ctstID`,`userID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `ctst_cat`
--

DROP TABLE IF EXISTS `ctst_cat`;
CREATE TABLE IF NOT EXISTS `ctst_cat` (
  `catID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `ctstID` int(10) unsigned NOT NULL,
  `name` varchar(72) DEFAULT NULL,
  `class` enum('power','glider','other') NOT NULL,
  `category` enum('primary','sportsman','intermediate','advanced','unlimited','4min','other') NOT NULL,
  `regAmt` smallint(5) unsigned DEFAULT NULL,
  `hasStudentReg` enum('y','n') NOT NULL DEFAULT 'n',
  `studentRegAmt` smallint(5) unsigned DEFAULT NULL,
  `hasTeamReg` enum('y','n') NOT NULL DEFAULT 'n',
  `teamRegAmt` smallint(5) unsigned DEFAULT NULL,
  `hasVoteJudge` enum('y','n') NOT NULL DEFAULT 'n',
  `maxVotes` smallint(5) unsigned DEFAULT NULL,
  `voteTeamOnly` enum('y','n') NOT NULL DEFAULT 'n',
  `voteByRegion` enum('y','n') NOT NULL DEFAULT 'n',
  `maxRegion` smallint(5) unsigned DEFAULT NULL,
  `voteDeadline` date DEFAULT NULL,
  `hasFourMinute` enum('y','n') NOT NULL DEFAULT 'n',
  `fourMinRegAmt` smallint(5) unsigned DEFAULT NULL,
  PRIMARY KEY (`catID`),
  KEY `ctstID` (`ctstID`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=686 ;

-- --------------------------------------------------------

--
-- Table structure for table `judge`
--

DROP TABLE IF EXISTS `judge`;
CREATE TABLE IF NOT EXISTS `judge` (
  `ctstID` int(10) unsigned NOT NULL,
  `givenName` varchar(72) DEFAULT NULL,
  `familyName` varchar(72) DEFAULT NULL,
  `contactPhone` char(16) DEFAULT NULL,
  `iacID` char(12) NOT NULL,
  `region` enum('northeast','southeast','midamerica','southcentral','northwest','southwest') DEFAULT NULL,
  `availableDate` date DEFAULT NULL,
  `voteCount` smallint(6) NOT NULL DEFAULT '0',
  UNIQUE KEY `iacID` (`iacID`,`ctstID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `pptxn`
--

DROP TABLE IF EXISTS `pptxn`;
CREATE TABLE IF NOT EXISTS `pptxn` (
  `txn_id` char(17) NOT NULL,
  `regID` int(10) unsigned NOT NULL,
  `pay_date` char(28) NOT NULL,
  `item_name` char(127) NOT NULL,
  `pay_amt` decimal(6,2) NOT NULL,
  `currency` char(3) NOT NULL,
  `payer_email` varchar(127) NOT NULL,
  `first_name` varchar(64) NOT NULL,
  `last_name` varchar(64) NOT NULL,
  UNIQUE KEY `txn_id` (`txn_id`),
  KEY `regID` (`regID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `practice_slot`
--

DROP TABLE IF EXISTS `practice_slot`;
CREATE TABLE IF NOT EXISTS `practice_slot` (
  `sessID` int(10) unsigned NOT NULL,
  `slotIndex` smallint(6) NOT NULL,
  `userID` int(10) unsigned NOT NULL,
  UNIQUE KEY `sessID` (`sessID`,`slotIndex`),
  KEY `sessID_2` (`sessID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `registrant`
--

DROP TABLE IF EXISTS `registrant`;
CREATE TABLE IF NOT EXISTS `registrant` (
  `userID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `accountName` char(32) NOT NULL,
  `password` char(40) NOT NULL,
  `updated` datetime NOT NULL,
  `admin` enum('y','n') NOT NULL DEFAULT 'n',
  `email` text NOT NULL,
  `givenName` varchar(72) DEFAULT NULL,
  `familyName` varchar(72) DEFAULT NULL,
  `contactPhone` char(16) DEFAULT NULL,
  `address` varchar(72) DEFAULT NULL,
  `city` varchar(24) DEFAULT NULL,
  `state` varchar(24) DEFAULT NULL,
  `country` varchar(24) DEFAULT NULL,
  `postalCode` char(12) DEFAULT NULL,
  `certType` enum('none','student','private','commercial','atp','sport','recreational') DEFAULT NULL,
  `certNumber` char(16) DEFAULT NULL,
  `eaaID` char(12) DEFAULT NULL,
  `iacID` char(12) DEFAULT NULL,
  `faiID` char(12) DEFAULT NULL,
  `judgeQualification` enum('none','regional','national') NOT NULL DEFAULT 'none',
  `shirtsize` enum('XS','S','M','L','XL','XXL') NOT NULL DEFAULT 'L',
  `iceName` varchar(72) DEFAULT NULL,
  `icePhone1` char(16) DEFAULT NULL,
  `icePhone2` char(16) DEFAULT NULL,
  PRIMARY KEY (`userID`),
  UNIQUE KEY `accountName` (`accountName`),
  KEY `accountName_2` (`accountName`,`password`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=985 ;

-- --------------------------------------------------------

--
-- Table structure for table `registration`
--

DROP TABLE IF EXISTS `registration`;
CREATE TABLE IF NOT EXISTS `registration` (
  `regID` int(10) unsigned NOT NULL,
  `catID` int(10) unsigned NOT NULL,
  `chapter` char(6) DEFAULT NULL,
  `teamAspirant` enum('y','n') NOT NULL DEFAULT 'n',
  `fourMinFree` enum('y','n') NOT NULL DEFAULT 'n',
  `currMedical` enum('y','n') NOT NULL DEFAULT 'n',
  `currBiAnn` enum('y','n') NOT NULL DEFAULT 'n',
  `currPacked` enum('y','n') NOT NULL DEFAULT 'n',
  `safety` varchar(72) DEFAULT NULL,
  `ownerPilot` enum('y','n') NOT NULL DEFAULT 'y',
  `ownerName` varchar(72) DEFAULT NULL,
  `ownerPhone` char(16) DEFAULT NULL,
  `ownerAddress` varchar(72) DEFAULT NULL,
  `ownerCity` varchar(24) DEFAULT NULL,
  `ownerCountry` varchar(24) DEFAULT NULL,
  `ownerState` varchar(24) DEFAULT NULL,
  `ownerPostal` char(12) DEFAULT NULL,
  `airplaneMake` varchar(24) DEFAULT NULL,
  `airplaneModel` varchar(24) DEFAULT NULL,
  `airplaneRegID` char(16) DEFAULT NULL,
  `airplaneColors` varchar(24) DEFAULT NULL,
  `airworthiness` enum('experimental','acrobatic') NOT NULL DEFAULT 'experimental',
  `engineMake` varchar(24) DEFAULT NULL,
  `engineModel` varchar(24) DEFAULT NULL,
  `engineHP` char(6) DEFAULT NULL,
  `currInspection` enum('y','n') NOT NULL DEFAULT 'n',
  `insCompany` varchar(24) DEFAULT NULL,
  `liabilityAmt` enum('y','n') NOT NULL DEFAULT 'n',
  `injuryAmt` enum('y','n') NOT NULL DEFAULT 'n',
  `insExpires` char(10) DEFAULT NULL,
  `isStudent` enum('y','n') NOT NULL DEFAULT 'n',
  `university` varchar(48) DEFAULT NULL,
  `program` varchar(32) DEFAULT NULL,
  `isFirstTime` enum('y','n') NOT NULL DEFAULT 'n',
  `paidAmt` int(11) DEFAULT NULL,
  `hasVotedJudge` enum('y','n') NOT NULL DEFAULT 'n',
  PRIMARY KEY (`regID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `reg_type`
--

DROP TABLE IF EXISTS `reg_type`;
CREATE TABLE IF NOT EXISTS `reg_type` (
  `regID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `userID` int(10) unsigned NOT NULL,
  `ctstID` int(10) unsigned NOT NULL,
  `compType` enum('regrets','competitor','volunteer') NOT NULL DEFAULT 'competitor',
  `answer` varchar(4096) DEFAULT NULL,
  PRIMARY KEY (`regID`),
  UNIQUE KEY `userID` (`userID`,`ctstID`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2957 ;

-- --------------------------------------------------------

--
-- Table structure for table `session`
--

DROP TABLE IF EXISTS `session`;
CREATE TABLE IF NOT EXISTS `session` (
  `sessID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `ctstID` int(10) unsigned NOT NULL,
  `practiceDate` date NOT NULL,
  `startTime` time NOT NULL,
  `endTime` time NOT NULL,
  `minutesPer` smallint(5) unsigned NOT NULL,
  `maxSlotsPer` smallint(5) unsigned DEFAULT NULL,
  PRIMARY KEY (`sessID`),
  KEY `ctstID` (`ctstID`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=123 ;

-- --------------------------------------------------------

--
-- Table structure for table `slot_restriction`
--

DROP TABLE IF EXISTS `slot_restriction`;
CREATE TABLE IF NOT EXISTS `slot_restriction` (
  `sessID` int(10) unsigned NOT NULL,
  `slotIndex` smallint(6) NOT NULL,
  `restrictionType` enum('class','category') DEFAULT 'class',
  `class` enum('power','glider','other') DEFAULT 'other',
  `catID` int(10) unsigned DEFAULT NULL,
  UNIQUE KEY `sessID` (`sessID`,`slotIndex`),
  KEY `sessID_2` (`sessID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `volunteer`
--

DROP TABLE IF EXISTS `volunteer`;
CREATE TABLE IF NOT EXISTS `volunteer` (
  `userID` int(10) unsigned NOT NULL,
  `catID` int(10) unsigned NOT NULL,
  `volunteer` set('judge','assistJudge','recorder','boundary','runner','deadline','timer','assistChief') DEFAULT NULL,
  UNIQUE KEY `userID` (`userID`,`catID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
