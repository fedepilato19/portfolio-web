const express = require('express');
const mysql = require('mysql2');
const cors = require('cors');  

const app = express();
const PORT = 8080;

// Abilita CORS per tutte le richieste 
app.use(cors());

// conversione delle richieste JSON
app.use(express.json());


// Avvio del server
app.listen(PORT, () => {
  console.log(`Server in ascolto sulla porta ${PORT}`);
});

// Configurazione della connessione al mio database 'ecommerce'
const ecommerceDb = mysql.createConnection({
  host: 'localhost',
  user: 'root',
  password: '',
  database: 'ecommerce'
});

// Connessione al database
ecommerceDb.connect((err) => { //controllo utilizzando una callback
  if (err) {
    console.error('Errore di connessione al database:', err); 
    process.exit(1); // Esce se la connessione fallisce e cioè se err non è nullo
  }
  console.log('Connesso al database MySQL'); //se err è nullo o undefined , la connessione è andata a buon fine
});

/*GESTIONE RICHIESTE DELLA MIA API*/ 

// Gestione della richiesta GET per ottenere gli articoli
app.get('/articoli', (req, res) => { //req è l'oggetto della richiesta, res l'oggetto della risposta
  ecommerceDb.query('SELECT * FROM articoli', (err, results) => {
    if (err)
    {
      console.error('Errore nella query:', err);
      return res.status(500).json({ error: 'Errore nel recupero dei dati' }); //con status(500) mi assicuro che in caso di errore si comprenda che derivab dal server
    }

    // Invia i dati come JSON
    res.json(results);
  });
});


//Gestione della richiesta POST per aggiungere un articolo
app.post('/articoli',(req,res) =>{
    const{nome,prezzo,descrizione,url_immagine,categoria}=req.body;
    // Validazione dei dati ricevuti
      if (!nome || !prezzo || !descrizione || !url_immagine || !categoria)
        {
            return res.status(400).json({ error: 'Tutti i campi sono obbligatori' });
        }

    const query ='INSERT INTO articoli (nome, prezzo, descrizione, url_immagine, categoria) VALUES(?, ?, ?, ?, ?)'; 
    ecommerceDb.execute(query,[nome,prezzo,descrizione,url_immagine,categoria],(err,results) =>{
      if (err)
        {
          console.error('Errore nell\'inserimento:', err);
          return res.status(500).json({ error: 'Errore nell\'inserimento dei dati' });
        }

        const newarticle = {
          id: results.insertId,
          nome,
          prezzo,
          descrizione,
          url_immagine,
          categoria
        };

        res.status(201).json(newarticle);
    });

});

//Gestione della richiesta PUT per aggiornare articolo

app.put('/articoli/:id', (req, res) => {
  const { id } = req.params; // Ottieni l'ID dall'URL
  const { nome, prezzo, descrizione, url_immagine, categoria } = req.body; // Ottieni i dati dal body

  // Validazione dei dati
  if (!nome || !prezzo || !descrizione || !url_immagine || !categoria) {
    return res.status(400).json({ error: 'Tutti i campi sono obbligatori' });
  }

  // Query SQL per aggiornare l'articolo
  const query = `
    UPDATE articoli 
    SET nome = ?, prezzo = ?, descrizione = ?, url_immagine = ?, categoria = ?
    WHERE id = ?
  `;

  ecommerceDb.execute(
    query, 
    [nome, prezzo, descrizione, url_immagine, categoria, id], 
    (err, results) => {
      if (err) {
        console.error('Errore nell\'aggiornamento:', err);
        return res.status(500).json({ error: 'Errore nell\'aggiornamento dei dati' });
      }

      if (results.affectedRows === 0) {
        return res.status(404).json({ error: 'Articolo non trovato' });
      }

      // Conferma l'aggiornamento
      res.json({
        updatedArticle: {
          id,
          nome,
          prezzo,
          descrizione,
          url_immagine,
          categoria
        }
      });
    }
  );
});

// Gestione della richiesta DELETE per eliminare un articolo
app.delete('/articoli/:id', (req, res) => {
  const { id } = req.params;

  const query = 'DELETE FROM articoli WHERE id = ?';
  ecommerceDb.execute(query, [id], (err, results) => {
    if (err) {
      console.error('Errore nell\'eliminazione:', err);
      return res.status(500).json({ error: 'Errore nell\'eliminazione dei dati' });
    }

    if (results.affectedRows === 0) {
      return res.status(404).json({ error: 'Articolo non trovato' });
    }

    // Conferma l'eliminazione
    res.json({ message: 'Articolo eliminato con successo' });


  });
});

//Carrello

app.post('/carrello', (req, res) => {
  const { articolo_id, quantita } = req.body;

  if (!articolo_id || !quantita) {
      return res.status(400).json({ error: 'Articolo e quantità sono obbligatori' });
  }

  // Controllo per vedere se l'articolo è già presente nel carrello
  const checkQuery = 'SELECT quantita FROM carrello WHERE articolo_id = ?';
  ecommerceDb.query(checkQuery, [articolo_id], (err, results) => {
      if (err) {
          console.error('Errore nel controllo dell\'articolo:', err);
          return res.status(500).json({ error: 'Errore nel controllo del carrello' });
      }

      if (results.length > 0) {
          // Se l'articolo esiste, aggiorna la quantità
          const newQuantity = results[0].quantita + quantita;
          const updateQuery = 'UPDATE carrello SET quantita = ? WHERE articolo_id = ?';
          ecommerceDb.execute(updateQuery, [newQuantity, articolo_id], (err) => {
              if (err) {
                  console.error('Errore nell\'aggiornamento della quantità:', err);
                  return res.status(500).json({ error: 'Errore nell\'aggiornamento del carrello' });
              }
              res.json({ message: 'Quantità aggiornata con successo', articolo_id, quantita: newQuantity });
          });
      } else {
          // Se l'articolo non esiste, lo aggiungo
          const insertQuery = 'INSERT INTO carrello (articolo_id, quantita) VALUES (?, ?)';
          ecommerceDb.execute(insertQuery, [articolo_id, quantita], (err, results) => {
              if (err) {
                  console.error('Errore nell\'aggiunta al carrello:', err);
                  return res.status(500).json({ error: 'Errore nell\'aggiunta al carrello' });
              }
              res.status(201).json({ message: 'Articolo aggiunto al carrello', id: results.insertId });
          });
      }
  });
});


// Recupera gli articoli nel carrello
app.get('/carrello', (req, res) => {
  const query = `
    SELECT carrello.articolo_id, articoli.nome, articoli.prezzo AS prezzo_unitario, carrello.quantita, (articoli.prezzo * carrello.quantita) AS prezzo_totale
    FROM carrello
    JOIN articoli ON carrello.articolo_id = articoli.id
  `;
  ecommerceDb.query(query, (err, results) => {
    if (err) {
      console.error('Errore nel recupero del carrello:', err);
      return res.status(500).json({ error: 'Errore nel recupero del carrello' });
    }
    res.json(results);
  });
});

// Gestione della richiesta DELETE per eliminare un articolo
app.delete('/carrello/:id', (req, res) => {
  const { id } = req.params;

  const query = 'DELETE FROM carrello WHERE articolo_id = ?';
  ecommerceDb.execute(query, [id], (err, results) => {
    if (err) {
      console.error('Errore nell\'eliminazione:', err);
      return res.status(500).json({ error: 'Errore nell\'eliminazione dei dati' });
    }

    if (results.affectedRows === 0) {
      return res.status(404).json({ error: 'Articolo non trovato' });
    }

    // Conferma l'eliminazione
    res.json({ message: 'Articolo eliminato con successo' });


  });
});

// Modifica la quantità di un articolo nel carrello
app.put('/carrello/:id', (req, res) => {
  const { id } = req.params;
  const { quantita } = req.body;

  if (quantita < 1) {
      return res.status(400).json({ error: 'La quantità deve essere maggiore di 0' });
  }

  const query = 'UPDATE carrello SET quantita = ? WHERE articolo_id = ?';
  ecommerceDb.execute(query, [quantita, id], (err, results) => {
      if (err) {
          console.error('Errore nell\'aggiornamento della quantità:', err);
          return res.status(500).json({ error: 'Errore nell\'aggiornamento della quantità' });
      }

      if (results.affectedRows === 0) {
          return res.status(404).json({ error: 'Articolo non trovato' });
      }

      res.json({ message: 'Quantità aggiornata con successo' });


  });
});











