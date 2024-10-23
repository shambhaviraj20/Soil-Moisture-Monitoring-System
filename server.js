const express = require('express');
const { Pool } = require('pg');
const path = require('path');

const app = express();

const pool = new Pool({
  user: 'sam',
  password: 'pass123',
  host: 'db',
  database: 'mydb',
  port: 5432,
});

app.use(express.json());
app.set('view engine', 'ejs');
app.set('views', path.join(__dirname, 'views'));

app.post('/update', (req, res) => {
  const { moisture } = req.body;

  if (typeof moisture !== 'number') {
    return res.status(400).json({ error: 'Invalid moisture value' });
  }

  const query = 'INSERT INTO moisture (value) VALUES (\$1)';

  pool.query(query, [moisture], (error) => {
    if (error) {
      console.error(error);
      res.status(500).json({ error: 'Failed to update moisture' });
    } else {
      res.sendStatus(200);
    }
  });
});

app.get('/get', (req, res) => {
  const query = 'SELECT value FROM moisture ORDER BY id DESC LIMIT 1';

  pool.query(query, (error, result) => {
    if (error) {
      console.error(error);
      res.status(500).json({ error: 'Failed to retrieve moisture' });
    } else {
      const latestMoisture = result.rows[0]?.value || '';
      res.send(String(latestMoisture));
    }
  });
});

app.get('/', (req, res) => {
  // Fetch the latest moisture value and render the index.ejs file
  pool.query('SELECT value FROM moisture ORDER BY id DESC LIMIT 1', (error, result) => {
    if (error) {
      console.error(error);
      res.status(500).json({ error: 'Failed to retrieve moisture' });
    } else {
      const latestMoisture = result.rows[0]?.value || '';
      res.render('index', { moistureValue: latestMoisture });
    }
  });
});

const port = 3000;
app.listen(port, () => {
  console.log(`Server is running on port ${port}`);
});
