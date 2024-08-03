<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Smart Plastic Bottle Redemption System</title>
    <style>
        body {
            font-family: 'Open Sans', Tahoma, Geneva, Verdana, sans-serif;
            margin: 0;
            padding: 0;
            display: flex;
            align-items: center;
            justify-content: center;
            background-image: url('website_bg.jpg');
            background-size: cover;
            background-position: center;
            background-repeat: no-repeat;
            height: 100vh;
        }
        .header {
            position: absolute;
            top: 20px;
            left: 20px;
            color: #FFFFFF;
            font-size: 2.5em;
            font-weight: bold;
            text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.2);
        }
        .container {
            background-color: rgba(255, 255, 255, 0.60);
            border-radius: 12px;
            padding: 30px;
            width: 90%;
            max-width: 900px;
            box-shadow: 0 6px 12px rgba(0, 0, 0, 0.2);
            display: flex;
            flex-direction: column;
            align-items: center;
            margin-top: 100px;
        }
        h1 {
            font-size: 2em;
            color: #333;
            margin: 0 0 20px 0;
            padding: 0;
            text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.1);
        }
        h2 {
            font-size: 1.5em;
            color: #007BFF;
            margin: 0 0 20px 0;
            padding: 0;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin: 20px 0;
            border-radius: 8px;
            overflow: hidden;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
            animation: slideIn 0.5s ease-out;
        }
        table, th, td {
            border: 1px solid #ddd;
            font-weight: bold;
        }
        th, td {
            padding: 12px;
            text-align: left;
        }
        th {
            background-color: #007BFF;
            color: #fff;
            font-weight: bold;
        }
        td {
            background-color: #fff;
        }
        tr:nth-child(even) td {
            background-color: #f9f9f9;
        }
        tr:hover td {
            background-color: #e0e0e0;
        }
        @keyframes slideIn {
            from { transform: translateY(-10px); opacity: 0; }
            to { transform: translateY(0); opacity: 1; }
        }
    </style>
    <script>
        function fetchDataAndUpdate() {
            var xhr = new XMLHttpRequest();
            xhr.open('GET', 'fetch_data.php', true);

            xhr.onload = function() {
                if (xhr.status >= 200 && xhr.status < 400) {
                    var data = JSON.parse(xhr.responseText);
                    updateTable(data);
                } else {
                    console.error("Failed to fetch data: " + xhr.status);
                }
            };

            xhr.onerror = function() {
                console.error("Error fetching data.");
            };

            xhr.send();
        }

        function updateTable(data) {
            var table = document.getElementById('data_table');
            var rows = table.getElementsByTagName('tr');

            while (table.rows.length > 1) {
                table.deleteRow(1);
            }

            data.forEach(function(item) {
                var newRow = table.insertRow();
                var cell1 = newRow.insertCell(0);
                var cell2 = newRow.insertCell(1);
                var cell3 = newRow.insertCell(2);

                cell1.innerText = item.id;
                cell2.innerText = item.name;
                cell3.innerText = item.value;
            });
        }

        fetchDataAndUpdate();
        setInterval(fetchDataAndUpdate, 2000); // Time --> Update Website
    </script>
</head>
<body>
    <div class="header">Smart Plastic Bottle Redemption System</div>
    <div class="container">
        <h1>Reward Dashboard</h1>
        <table id="data_table">
            <tr>
                <th>ID</th>
                <th>User</th>
                <th>Available points</th>
            </tr>
        </table>
    </div>
</body>
</html>
//By E.D.K.Chamara