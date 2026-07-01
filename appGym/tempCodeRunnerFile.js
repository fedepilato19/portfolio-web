//Funzione per ottenere gli esercizi dal database
app.get('/getSavedExercises', (req, res) => {
    const query = 'SELECT * FROM piano_workout';  // Modifica la query per recuperare gli esercizi
    db.query(query, (err, result) => {
        if (err) {
            console.error('Errore durante il recupero degli esercizi:', err);
            return res.status(500).send('Errore del server');
        }
        return res.json({status: true, result});
    });
});