import mysql.connector

def record_user_points(name):
    try:
        # Connect to the MySQL database
        conn = mysql.connector.connect(
            host="localhost",
            user="root",
            password="",  # Enter your MySQL root password
            database="testdb"
        )

        # Create a cursor object
        cursor = conn.cursor()

        # SQL query to insert data
        sql = "INSERT INTO data (name, value) VALUES (%s, %s)"
        values = (name, 10) # Update this if need a different score or value

        # Execute the query
        cursor.execute(sql, values)

        # Commit the transaction
        conn.commit()

        # Close the cursor and connection
        cursor.close()
        conn.close()

        print("Data inserted successfully!")
        
    except mysql.connector.Error as err:
        print(f"Error: {err}")