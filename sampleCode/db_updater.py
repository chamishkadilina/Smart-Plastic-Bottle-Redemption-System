import mysql.connector

# Database updating function
def record_user_points(user_name):
        
    # Connect to the MySQL database
    conn = mysql.connector.connect(
        host="localhost",
        user="root",
        password="",  # Enter your MySQL root password
        database="bottle_db"
    )

    # Create a cursor object
    cursor = conn.cursor()

    # Check if user already exists
    cursor.execute("SELECT value FROM user_data WHERE name = %s", (user_name,))
    existing_user = cursor.fetchone()

    if existing_user:
        # User exists, increment their value
        new_value = existing_user[0] + 10  # Increment the current value by 10
        sql = "UPDATE user_data SET value = %s WHERE name = %s"
        values = (new_value, user_name)
    else:
        # User does not exist, insert new record
        sql = "INSERT INTO user_data (name, value) VALUES (%s, %s)"
        values = (user_name, 10)

    # Execute the query
    cursor.execute(sql, values)

    # Commit the transaction
    conn.commit()

    # Close the cursor and connection
    cursor.close()
    conn.close()

    print("Data processed successfully!")
