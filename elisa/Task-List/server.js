const express = require('express');
const cors = require('cors');
const mysql = require('mysql2');
const app = express();
const port = 3000;

// Abilitazione CORS e configurazione JSON per POST/PUT
app.use(cors());
app.use(express.json());

// Configurazione della connessione al database MySQL
const db = mysql.createConnection({
  host: 'localhost',
  user: 'root', // Utente MySQL
  password: 'Database2024.', // Password MySQL
  database: 'lista', // Nome del database
});

// Verifica della connessione al database
db.connect(err => {
  if (err) {
    console.error('Errore di connessione al database:', err.stack);
    return;
  }
  console.log('Connesso al database');
});

// Funzione per formattare la data in formato 'YYYY-MM-DD'
const formatDate = date => new Date(date).toISOString().split('T')[0];

// GET per ottenere le tasks con eventuali filtri
app.get('/tasks', (req, res) => {
  const { categoria, scadute } = req.query;
  let query = 'SELECT * FROM tasks';
  const conditions = [];

  if (categoria) {
    conditions.push('categoria = ?');
  }
  if (scadute === 'true') {
    conditions.push('data_scadenza < CURDATE()');
  }
  
  if (conditions.length > 0) {
    query += ' WHERE ' + conditions.join(' AND ');
  }

  db.query(query, [categoria], (err, results) => {
    if (err) return res.status(500).json({ error: 'Errore nel caricamento delle tasks' });
    res.json(results);
  });
});

// PUT per aggiornare una task
app.put('/tasks/:id', (req, res) => {
  const { titolo, data_scadenza, categoria } = req.body;
  const taskId = req.params.id;

  if (!titolo || !data_scadenza || !categoria) {
    return res.status(400).json({ error: 'Tutti i campi sono obbligatori' });
  }

  const formattedDate = formatDate(data_scadenza);
  const query = 'UPDATE tasks SET titolo = ?, data_scadenza = ?, categoria = ? WHERE id = ?';
  
  db.query(query, [titolo, formattedDate, categoria, taskId], (err, results) => {
    if (err) return res.status(500).json({ error: 'Errore nell\'aggiornamento della task' });
    if (results.affectedRows === 0) return res.status(404).json({ error: 'Task non trovata' });
    res.json({ id: taskId, titolo, data_scadenza: formattedDate, categoria });
  });
});

// PUT per segnare una task come completata o annullare il completamento
app.put('/tasks/completa/:id', (req, res) => {
  const taskId = req.params.id;
  const completataIl = req.body.completata_il ? formatDate(new Date()) : null;

  const query = 'UPDATE tasks SET completata_il = ? WHERE id = ?';
  db.query(query, [completataIl, taskId], (err, results) => {
    if (err) return res.status(500).json({ error: 'Errore nel completamento della task' });
    if (results.affectedRows === 0) return res.status(404).json({ error: 'Task non trovata' });
    res.json({ id: taskId, completata_il: completataIl });
  });
});

// POST per aggiungere una nuova task
app.post('/tasks', (req, res) => {
  const { titolo, data_scadenza, categoria } = req.body;

  if (!titolo || !data_scadenza || !categoria) {
    return res.status(400).json({ error: 'Tutti i campi sono obbligatori' });
  }

  const formattedDate = formatDate(data_scadenza);
  const query = 'INSERT INTO tasks (titolo, data_scadenza, categoria) VALUES (?, ?, ?)';

  db.query(query, [titolo, formattedDate, categoria], (err, results) => {
    if (err) return res.status(500).json({ error: 'Errore nell\'aggiunta della task' });
    res.status(201).json({ id: results.insertId, titolo, data_scadenza: formattedDate, categoria });
  });
});

// DELETE per eliminare una task
app.delete('/tasks/:id', (req, res) => {
  const taskId = req.params.id;
  const query = 'DELETE FROM tasks WHERE id = ?';

  db.query(query, [taskId], (err, results) => {
    if (err) return res.status(500).json({ error: 'Errore nell\'eliminazione della task' });
    if (results.affectedRows === 0) return res.status(404).json({ error: 'Task non trovata' });
    res.status(204).send();
  });
});

// Avvio del server
app.listen(port, () => {
  console.log(`Server in esecuzione sulla porta ${port}`);
});