import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

if __name__ == "__main__":
    app.run(host = "0.0.0.0", port = int("5000"), debug = True)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Remember logged in user
    user_id = session["user_id"]

    # Query for users stock info
    user_transactions = db.execute("SELECT symbol, name, SUM(shares) AS total_shares, price FROM transactions WHERE user_id = ? GROUP BY symbol", user_id)

    # Query for users current cash
    user_cash = db.execute("SELECT cash FROM users where id = ?", user_id)[0]["cash"]

    # Add up the users total assets
    total = user_cash
    for stock in user_transactions:
        total += stock["price"] * stock["total_shares"]

    # Render users portfolio
    return render_template("index.html", user_transactions = user_transactions, user_cash = usd(user_cash), usd = usd, total = usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Define variables
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide stock symbol", 400)

        # Look up stock quote via IEX API
        stock = lookup(symbol.upper())

        # Ensure stock exists
        if stock == None:
            return apology("stock symbol does not exist", 400)

        # Determine the transaction value of a given buy
        transaction_value = shares * stock["price"]

        # Ensure user has enough money for purchase
        user_id = session["user_id"]
        user_current_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
        if user_current_cash < transaction_value:
            return apology("too poor, purchase failed :(", 400)

        # Update the users cash
        user_update_cash = user_current_cash - transaction_value
        db.execute("UPDATE users SET cash = ? WHERE id = ?", user_update_cash, user_id)

        # Determine the date/time of purchase
        date = datetime.datetime.now()

        # Update the transctions database of the purchase
        db.execute("INSERT INTO transactions (user_id, type, symbol, name, shares, price, timestamp) VALUES (?,?,?,?,?,?,?)", user_id, "BUY", stock["symbol"], stock["name"], shares, stock["price"], date)

        # Inform user they've made a purchase
        flash("Stock bought!")

        # Redirect user to homepage
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Remember which user has logged in
    user_id = session["user_id"]

    # Query users transaction history
    user_transactions = db.execute("SELECT type, symbol, price, shares, timestamp FROM transactions WHERE user_id = ? ORDER BY timestamp DESC", user_id)

    # Render the users transaction history
    return render_template("history.html", user_transactions = user_transactions, usd = usd)
 

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Define variables
        symbol = request.form.get("symbol")

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide stock symbol", 400)

        # Look up stock quote via IEX API
        stock = lookup(symbol.upper())

        # Ensure stock exists
        if stock == None:
            return apology("stock symbol does not exist", 400)

        # Redirect user to quoted page
        return render_template("quoted.html", symbol = stock["symbol"], name = stock["name"], price = stock["price"])

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Define variables
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 400)

        # Ensure password was submitted
        if not password:
            return apology("must provide password", 400)

        # Ensure confirmation was submitted
        if not confirmation:
            return apology("must provide confirmation", 400)

        # Ensure password and confirmation match
        if password != confirmation:
            return apology("passwords don't match", 400)

        # Hash password
        hash = generate_password_hash(password)

        try:
            # Insert username and hashed password into database
            new_user = db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash)
        except:
            # If username already exists
            return apology("username already exists", 400)

        # Remember logged in user
        session["user_id"] = new_user

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Remember logged in user
        user_id = session["user_id"]

        # Define variables
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide stock symbol", 400)

        # Look up stock quote via IEX API
        stock = lookup(symbol.upper())

        # Ensure stock exists
        if stock == None:
            return apology("stock symbol does not exist", 400)

        # Ensure shares are not greater then the users current shares
        user_current_shares = db.execute("SELECT shares FROM transactions WHERE user_id = ? AND symbol = ? GROUP BY symbol", user_id, symbol)[0]["shares"]
        if shares > user_current_shares:
            return apology("too many shares", 400)

        # Query users current cash value
        user_current_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

        # Determine the transaction value for selling
        transaction_value = shares * stock["price"]

        # Update the users cash
        user_update_cash = user_current_cash + transaction_value
        db.execute("UPDATE users SET cash = ? WHERE id = ?", user_update_cash, user_id)

        # Determine the date/time of selling
        date = datetime.datetime.now()

        # Update the transactions database of the selling
        db.execute("INSERT INTO transactions (user_id, type, symbol, name, shares, price, timestamp) VALUES (?,?,?,?,?,?,?)", user_id, "SELL", stock["symbol"], stock["name"], (-1)*shares, stock["price"], date)

        # Inform user they've sold stock
        flash("Stock sold!")

        # Redirect user to homepage
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Remember which user has logged in
        user_id = session["user_id"]

        # Query users current stock symbols
        user_symbols = db.execute("SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol", user_id)

        # Render the sell template
        return render_template("sell.html", symbols = [row["symbol"] for row in user_symbols])


@app.route("/changepassword", methods=["GET", "POST"])
def change_password():
    """Allow user to change their password"""

    # Remember logged in user
    user_id = session["user_id"]

    # User reached route via GET (as by clicking a link or via redirect)
    if request.method == "GET":
        return render_template("changepassword.html")

    # User reached route via POST (as by submitting a form via POST)
    current_password = request.form.get("current_password")
    new_password = request.form.get("new_password")
    new_confirmation = request.form.get("new_confirmation")

    # Ensure the current password was submitted
    if not current_password:
        return apology("input your current password")

    # Ensure a new password was submitted
    if not new_password:
        return apology("input your new password", 400)

    # Ensure new confirmation was submitted
    if not new_confirmation:
        return apology("must provide confirmation", 400)

    # Ensure new password and confirmation match
    if new_password != new_confirmation:
        return apology("new passwords don't match", 400)

    # Query for users current password
    old_password = db.execute("SELECT hash FROM users WHERE id = ?", user_id)
    if len(old_password) != 1 or not check_password_hash(old_password[0]["hash"], current_password):
        return apology("invalid username and/or password", 400)

    # Hash new password
    new_hash = generate_password_hash(new_password)

    # Update users new password
    db.execute("UPDATE users SET hash = ? WHERE id = ?", new_hash, user_id)

    # Redirect the user to login form
    return redirect("/logout")
