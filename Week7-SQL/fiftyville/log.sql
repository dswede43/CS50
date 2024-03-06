--Fiftyville
--SQL queries to solve a mystery in Fiftyville

-- Q1: Crime scene description
-- Find the description of the crime report
SELECT description FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28
AND street = "Humphrey Street";


-- Q2: Witness interviews
-- Find the transcripts from witness interviews
SELECT transcript FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28
AND transcript LIKE "%bakery%";


-- Q3: Interview #1 follow-up
-- Find the names of people registered to license plates of cars that
-- were leaving the bakery parking lot between 10:15-10:25am
-- TABLE 1
SELECT bakery_security_logs.activity,
bakery_security_logs.license_plate,
people.name FROM people
JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
WHERE bakery_security_logs.year = 2021
AND bakery_security_logs.month = 7
AND bakery_security_logs.day = 28
AND bakery_security_logs.hour = 10
AND bakery_security_logs.minute BETWEEN 15 AND 25
AND activity = "exit";


-- Q4: Interview #2 follow-up
-- Find the names of people with bank account numbers that withdrew
-- money from the ATM on Leggett Street on the day of the crime
-- TABLE 2
SELECT atm_transactions.account_number,
atm_transactions.transaction_type,
people.name FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
WHERE atm_transactions.year = 2021
AND atm_transactions.month = 7
AND atm_transactions.day = 28
AND atm_transactions.atm_location = "Leggett Street"
AND atm_transactions.transaction_type = "withdraw";


-- Q5: Interview #3 follow-up (part 1)
-- Update the phone calls table to show the names of people
-- associated to each phone number
-- Add column for caller name
ALTER TABLE phone_calls
ADD caller_name TEXT;

-- Add column for receiver name
ALTER TABLE phone_calls
ADD receiver_name TEXT;

-- Find the phone numbers of callers and recievers with phone calls
-- less than 1 min on the day of the crime
-- TABLE 3
SELECT caller, caller_name,
receiver, receiver_name
FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28
AND duration < 60;

-- Update phone calls table with the names of callers
UPDATE phone_calls
SET caller_name = people.name FROM people
WHERE phone_calls.caller = people.phone_number;

-- Update phone calls table with the names of recievers
UPDATE phone_calls
SET receiver_name = people.name FROM people
WHERE phone_calls.receiver = people.phone_number;


-- Q6: Interview #3 follow-up (part 2)
-- Find the origin and desination airport city for the
-- earliest flight the day after the crime
SELECT id, hour, minute,
origin_airport_id, destination_airport_id FROM flights
WHERE year = 2021 AND month = 7 AND day = 29
ORDER BY hour ASC LIMIT 1;

-- Update the flights table with the city for the origin airport
UPDATE flights
SET origin_airport_id = airports.city FROM airports
WHERE flights.origin_airport_id = airports.id;

-- Update the flights table with the city for the destination airport
UPDATE flights
SET destination_airport_id = airports.city FROM airports
WHERE flights.destination_airport_id = airports.id;

-- Find the names and passport numbers of all passengers
-- flying to New York City the day after the crime
-- TABLE 4
SELECT flights.destination_airport_id, name,
people.passport_number FROM people
JOIN flights ON flights.id = passengers.flight_id
JOIN passengers ON people.passport_number = passengers.passport_number
WHERE flights.id = 36
ORDER BY flights.hour ASC;


-- Q7: Overlap the names from all the tables created from past queries
-- The name of the person appearing in all 4 tables is the theif
SELECT name FROM people -- TABLE 4 QUERY
JOIN flights ON flights.id = passengers.flight_id
JOIN passengers ON people.passport_number = passengers.passport_number
WHERE flights.year = 2021 AND flights.month = 7
AND flights.day = 29 AND flights.id = 36
AND name IN -- TABLE 3 QUERY
(SELECT caller_name FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28
AND duration < 60)
AND name IN -- TABLE 2 QUERY
(SELECT people.name FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
WHERE atm_transactions.year = 2021
AND atm_transactions.month = 7
AND atm_transactions.day = 28
AND atm_transactions.atm_location = "Leggett Street"
AND atm_transactions.transaction_type = "withdraw")
AND name IN -- TABLE 1 QUERY
(SELECT people.name FROM people
JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
WHERE bakery_security_logs.year = 2021
AND bakery_security_logs.month = 7
AND bakery_security_logs.day = 28
AND bakery_security_logs.hour = 10
AND bakery_security_logs.minute BETWEEN 15 AND 25
AND activity = "exit");
