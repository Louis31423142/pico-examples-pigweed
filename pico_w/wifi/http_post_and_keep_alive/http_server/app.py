from flask import Flask, render_template, request

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html", status="waiting...")

@app.route("/get_button", methods=["GET"])
def get_button():
    return render_template("index.html", status="GET!")

@app.route("/post_button", methods=["POST"])
def post_button():
    data=request.data
    print("recieved data: ", data)
    return render_template("index.html", status="POST!")

#run with: python -m waitress --listen={your computer's ip address}:5000 app:app
