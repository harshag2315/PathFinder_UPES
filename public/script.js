function findPath() {
    const start = document.getElementById('start').value;
    const end = document.getElementById('end').value;
    const type =document.getElementById('transport').value;
    const time = new Date().getHours();
    const weather=document.getElementById('weather_type').value;
    
    if (!start || !end ) {
        alert('Please enter both start and end nodes.');
        return;
    }
    if(!type){
        alert('Please pick type of way you want to travel');
        return;
    }
    if(!weather){
        alert('Weather field not field');
    }

    document.getElementById('result').textContent = 'Calculating...';

    fetch('/findpath', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ start, end, type, time, weather }),
    })
    .then(response => response.json())
    .then(data => {
        if (data.error) {
            document.getElementById('result').textContent = `Error: ${data.error}`;
        } else {
            document.getElementById('result').textContent = data.result;
        }
    })
    
    .catch(error => {
        console.error('Error:', error);
        document.getElementById('result').textContent = 'An error occurred while communicating with the server.';
    });
}