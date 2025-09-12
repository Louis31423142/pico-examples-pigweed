# Pico post and keep alive 

Start running a http server on your computer, and then power your pico and it will send a post request followed by a second post request, reusing the same connection. 

To start running the http server install waitress and flask, then navigate to where app.py is and run the command python -m waitress --listen={your computer's ip address}:5000 app:app

You should see a couple of messages appear from the pico client's post requests.
