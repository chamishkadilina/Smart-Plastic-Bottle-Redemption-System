<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Smart Plastic Bottle Redemption System</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
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
            margin-top: 100px; /* Space for the title */
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
</head>
<body>
    <div class="header">Smart Plastic Bottle Redemption System</div>
    <div class="container">
        <h1>Reward Dashboard</h1>
        
        <?php
        $servername = "localhost";
        $username = "root";
        $password = ""; // Enter your MySQL root password
        $dbname = "bottle_db";

        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);

        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        }

        $sql = "SELECT id, name, value FROM user_data";
        $result = $conn->query($sql);

        if ($result->num_rows > 0) {
            echo "<table><tr><th>ID</th><th>User</th><th>Available points</th></tr>";
            // Output data of each row
            while($row = $result->fetch_assoc()) {
                echo "<tr><td>" . $row["id"]. "</td><td>" . $row["name"]. "</td><td>" . $row["value"]. "</td></tr>";
            }
            echo "</table>";
        } else {
            echo "<p>No results found.</p>";
        }
        $conn->close();
        ?>
    </div>
</body>
</html>
