const express = require('express');
const { exec } = require('child_process');
const path = require('path');

const app = express();
const port = 3001;  // Changed to 3001

app.use(express.static('public'));
app.use(express.json());

app.post('/findpath', (req, res) => {
    const { start, end, type, time, weather } = req.body;
    const pathfinderExecutable = path.join(__dirname, process.platform === 'win32' ? 'pathfinder.exe' : 'pathfinder');

    exec(`${pathfinderExecutable} ${start} ${end} ${type} ${time} ${weather}`, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error: ${error.message}`);
            return res.status(500).json({ error: 'Internal Server Error' });
        }
        if (stderr) {
            console.error(`stderr: ${stderr}`);
            return res.status(500).json({ error: 'Internal Server Error' });
        }
        res.json({ result: stdout.trim() });
    });
});

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});