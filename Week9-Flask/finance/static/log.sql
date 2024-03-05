-- SQL command to create transactions table
CREATE TABLE transactions (id INTEGER, user_id NUMERIC NOT NULL, type TEXT NOT NULL, symbol TEXT NOT NULL, name TEXT NOT NULL, shares INTEGER NOT NULL, price NUMERIC NOT NULL, timestamp DEFAULT CURRENT_TIMESTAMP, PRIMARY KEY(id), FOREIGN KEY(user_id) REFERENCES users(id));
