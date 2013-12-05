-- phpMyAdmin SQL Dump
-- version 3.5.8.1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Dec 04, 2013 at 12:35 AM
-- Server version: 5.5.34
-- PHP Version: 5.4.21

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `Starclash`
--
USE Starclash;

DROP TABLE IF EXISTS `Team_2`;
DROP TABLE IF EXISTS `Team_1`;
DROP TABLE IF EXISTS `Player`;
DROP TABLE IF EXISTS `Lobby`;

-- --------------------------------------------------------

--
-- Table structure for table `Lobby`
--

CREATE TABLE IF NOT EXISTS `Lobby` (
  `lobby_id` int(11) NOT NULL,
  `team_1_id` int(11) NOT NULL,
  `team_2_id` int(11) NOT NULL,
  `num_players` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Player`
--

CREATE TABLE IF NOT EXISTS `Player` (
  `player_id` int(11) NOT NULL,
  `username` varchar(50) NOT NULL,
  `password` varchar(50) NOT NULL,
  `fname` varchar(50) NOT NULL,
  `lname` varchar(50) NOT NULL,
  `email` varchar(100) NOT NULL,
  `total_kills` int(11) NOT NULL,
  `shots_fired` int(11) NOT NULL,
  `total_deaths` int(11) NOT NULL,
  `kill_death_ratio` decimal(10,0) NOT NULL,
  `accuracy` int(11) NOT NULL,
  `wins` int(11) NOT NULL,
  `losses` int(11) NOT NULL,
  `experience_points` int(11) NOT NULL,
  `captures` int(11) NOT NULL,
  `rank` varchar(50) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `Player`
--

INSERT INTO `Player` (`player_id`, `username`, `password`, `fname`, `lname`, `email`, `total_kills`, `shots_fired`, `total_deaths`, `kill_death_ratio`, `accuracy`, `wins`, `losses`, `experience_points`, `captures`, `rank`) VALUES
(1, 'johndoe', 'password', 'John', 'Doe', 'johndoe@gmail.com', 100, 1000, 50, 2, 80, 30, 40, 2000, 5, 'captain'),
(2, 'janesmith', 'password1', 'Jane', 'Smith', 'janesmith@gmail.com', 70, 1400, 80, 4, 50, 30, 20, 5000, 20, 'novice');

-- --------------------------------------------------------

--
-- Table structure for table `Team_1`
--

CREATE TABLE IF NOT EXISTS `Team_1` (
  `team_1_id` int(11) NOT NULL,
  `num_players` int(11) NOT NULL,
  `color` varchar(10) NOT NULL,
  `lobby_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Team_2`
--

CREATE TABLE IF NOT EXISTS `Team_2` (
  `team_2_id` int(11) NOT NULL,
  `num_players` int(11) NOT NULL,
  `color` varchar(10) NOT NULL,
  `lobby_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
