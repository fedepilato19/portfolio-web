-- MySQL dump 10.13  Distrib 8.0.40, for Win64 (x86_64)
--
-- Host: 127.0.0.1    Database: lista
-- ------------------------------------------------------
-- Server version	5.5.5-10.4.32-MariaDB

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `tasks`
--

DROP TABLE IF EXISTS `tasks`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tasks` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `titolo` varchar(255) NOT NULL,
  `status` varchar(20) DEFAULT NULL,
  `data_scadenza` date NOT NULL,
  `categoria` varchar(255) NOT NULL,
  `completata_il` date DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=90 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tasks`
--

LOCK TABLES `tasks` WRITE;
/*!40000 ALTER TABLE `tasks` DISABLE KEYS */;
INSERT INTO `tasks` VALUES (1,'Andare a danza','Non completato','2024-11-12','Salute','2024-11-22'),(4,'Chiamare il dottore per un appuntamento','Non completato','2024-11-28','Salute',NULL),(5,'Prenotare il volo per le vacanze','Non completato','2024-12-01','Vacanze','2024-11-26'),(6,'Fare il backup dei dati importanti','Non completato','2024-12-02','Lavoro',NULL),(7,'Comprare un regalo per il compleanno di Mario','Non completato','2024-12-05','Personale',NULL),(8,'Leggere il capitolo 5 del libro','Non completato','2024-12-10','Relax',NULL),(9,'Rispondere alle email di lavoro','Non completato','2024-12-12','Lavoro','2024-11-26'),(10,'Organizzare una cena con amici','Non completato','2024-12-15','Sociale',NULL),(11,'Fare un controllo auto','Non completato','2024-12-17','Auto',NULL),(12,'Rivedere il progetto con il team','Non completato','2024-12-20','Lavoro',NULL),(13,'Aggiornare il CV','Non completato','2024-12-22','Lavoro',NULL),(14,'Comprare i biglietti per il concerto','Non completato','2024-12-25','Sociale',NULL),(15,'Prenotare tabolo al ristorante','Non completato','2024-12-28','Sociale',NULL),(79,'piangersi addosso',NULL,'2024-11-30','Personale',NULL),(80,'Fare i compiti',NULL,'2024-12-05','Scuola',NULL),(82,'Prelevare in banca',NULL,'2024-11-30','Personale',NULL),(83,'Comprare scarpe',NULL,'2024-11-30','shopping',NULL),(84,'Comprare vestito',NULL,'2024-11-18','shopping',NULL),(85,'Andare a lezione di musica',NULL,'2024-11-24','Scuola',NULL),(86,'comprare croccantini per teddy',NULL,'2024-12-27','Personale',NULL);
/*!40000 ALTER TABLE `tasks` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2024-12-03 10:02:08
