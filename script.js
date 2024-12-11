// Handle Login
document.getElementById('login-form').addEventListener('submit', function (e) {
    e.preventDefault();
    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;

    if (username === "admin" && password === "password") {
        document.getElementById('login-page').style.display = 'none';
        document.getElementById('main-page').style.display = 'block';
        startRealTimeUpdates();
    } else {
        alert("Invalid login credentials!");
    }
});

// Show Sections
function showSection(sectionId) {
    const sections = document.querySelectorAll('main > section');
    sections.forEach(section => {
        section.style.display = 'none';
    });
    document.getElementById(sectionId).style.display = 'block';
}

// Initialize Charts
let temperatureChart, humidityChart, pressureChart;
function createChart(chartId, label, data, color) {
    const ctx = document.getElementById(chartId).getContext('2d');
    return new Chart(ctx, {
        type: 'line',
        data: {
            labels: Array.from({ length: 6 }, (_, i) => `${i + 1}:00`), 
            datasets: [{
                label: label,
                data: data,
                borderColor: color,
                fill: false,
            }]
        },
        options: {
            responsive: true,
            plugins: {
                legend: {
                    display: true,
                    position: 'top'
                }
            }
        }
    });
}

function updateChart(chart, newValue) {
    const now = new Date();
    const currentHour = `${now.getHours()}:${String(now.getMinutes()).padStart(2, '0')}`;
    if (chart.data.labels.length >= 6) {
        chart.data.labels.shift(); 
        chart.data.datasets[0].data.shift(); 
    }
    chart.data.labels.push(currentHour); 
    chart.data.datasets[0].data.push(newValue); 
    chart.update(); 
}


function startRealTimeUpdates() {
    temperatureChart = createChart('temperatureChart', 'Temperature 10 (°C)', [20, 21, 22, 23, 24, 25], 'red');
    humidityChart = createChart('humidityChart', 'Humidity (%)', [60, 62, 64, 65, 63, 61], 'blue');
    pressureChart = createChart('pressureChart', 'Pression (hPa)', [1010, 1012, 1011, 1013, 1014, 1012], 'green');

    setInterval(() => {
        const temp = Math.random() * (25 - 20) + 20; 
        const hum = Math.random() * (70 - 60) + 60; 
        const press = Math.random() * (1015 - 1010) + 1010; 
        updateChart(temperatureChart, temp.toFixed(2));
        updateChart(humidityChart, hum.toFixed(2));
        updateChart(pressureChart, press.toFixed(2));

        document.getElementById('temperatureValue').textContent = `${temp.toFixed(2)} °C`;
        document.getElementById('humidityValue').textContent = `${hum.toFixed(2)} %`;
        document.getElementById('pressureValue').textContent = `${press.toFixed(2)} hPa`;

    }, 5000); 
}
