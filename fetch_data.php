<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "bottle_db";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// SQL query to select data ordered by id
$sql = "SELECT id, name, value FROM user_data ORDER BY id ASC";
$result = $conn->query($sql);

$data = array();
if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        $data[] = $row;
    }
}
$conn->close();

header('Content-Type: application/json');
echo json_encode($data);
?>
