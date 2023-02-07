import sqlite3
from flask import Flask, g, render_template, request, redirect, session
# from flask_bcrypt import Bcrypt

DATABASE = './assignment3.db'

app = Flask(__name__)
app.secret_key = "super secret key"
# bcrypt = Bcrypt(app)

#connect db 
def get_db():
    db = getattr(g, '_database', None)
    if db is None:
        db = g._database = sqlite3.connect(DATABASE)
    return db

#return as dict
def query_db(query, args=(), one = False):
    cur = get_db().execute(query, args)
    rv = cur.fetchall()
    cur.close()
    return (rv[0] if rv else None) if one else rv

def operate1_db(query, args=()):
    db = get_db()
    cur = db.cursor()
    cur.execute(query, args)
    db.commit()
    cur.close()
    db.close()

def operate2_db(query, args=()):
    db = get_db()
    cur = db.cursor()
    cur.execute(query, args)
    db.commit()
    cur.close()


def make_dicts(cursor, row):
    return dict((cursor.description[idx][0], value)
                for idx, value in enumerate(row))

# def postmark():
#     if 'username' not in session:
#         return redirect('/login') 
#     if session['type'] == 0:
#         return redirect('/')
#     if not request.args:
#         db = get_db()
#         db.row_factory = make_dicts
#         students = query_db('SELECT username FROM User WHERE type = 0')
#         worknames = query_db('SELECT DISTINCT workname FROM Mark ORDER BY workname asc')
#         db.close()
#         return render_template('postmark.html', student_names=students, worknames=worknames)
#     else:
#         if request.args['workname'] == "" or request.args['grade'] == "": 
#             return render_template('postmark.html', error=True)

#         workname = request.args['workname']
#         grade = float(request.args['grade'])
#         username = request.args['username']

#         HaveGrade = query_db('SELECT username, workname FROM Mark WHERE username=? AND workname=?',[username, workname], one=True)
#         if (HaveGrade):
#             operate2_db('DELETE FROM Mark WHERE workname=? AND username=?', [workname, username])
#         operate1_db('INSERT INTO Mark (workname, grade, username) VALUES (?, ?, ?)',(workname, grade, username))

@app.teardown_appcontext
def close_connection(exception):
    db = getattr(g, '_database', None)
    if db is not None:
        db.close()

@app.route('/')
def home():
    if 'username' not in session:
        return redirect('/login')

    return render_template('index.html', username=session['username'], type=session['type'])

@app.route('/register')
def register():

    if not request.args:
        return render_template('register.html')

    username = request.args['username']
    password = request.args['password']
    # password = bcrypt.generate_password_hash(request.args['password']).decode('utf-8')
    the_type = request.args['type']
    if len(request.args['type'])  == 0 or the_type == "Register as:":
        return render_template('register.html', warning = True)


    if len(username) < 1 or len(password)< 1  or len(the_type)< 1:
        return render_template('register.html', warning = True)

    db = get_db()
    db.row_factory = make_dicts
    user = query_db('SELECT * FROM User WHERE username=?',
                    [username], one=True)
    
    if user:
        return render_template('register.html', error = True)
    else:
        operate1_db('INSERT INTO User VALUES(?, ?, ?)', (username, password, the_type))
        return redirect('/login')

@app.route('/login')
def login():

    if len(request.args) == 0:
        return render_template('login.html')

    username = request.args['username']
    password = request.args['password']

    db = get_db()
    db.row_factory = make_dicts
    user = query_db('SELECT * FROM User WHERE username=? AND password=?', [username, password], one=True)
    db.close()

    if user:
        session['username'] = user['username']
        if  user['type'] == 1: 
            session['type'] = True 
        else:
            session['type'] = False
        return redirect('/')

    else:
        return render_template('login.html', error=True)

@app.route('/marks')
def marks():
    if 'username' not in session:
        return redirect('/login') 
    
    db = get_db()
    db.row_factory = make_dicts
    remarks = query_db('SELECT * FROM Mark, Complete WHERE Mark.markid = Complete.markid')

    if session['type']:
        marks = query_db('SELECT * FROM Mark')
        remarks = query_db('SELECT * FROM Mark, Complete WHERE Mark.markid = Complete.markid')
        db.close()
        return render_template('all_marks.html', username=session['username'], marks=marks, admin=session['type'])
    else:
        marks = query_db('SELECT * FROM Mark WHERE username=?', [session['username']])
        db.close()
        return render_template('marks.html', username=session['username'], marks=marks, remarks = remarks, admin=session['type'])

@app.route('/all_marks')
def all_marks():
    if 'username' not in session:
        return redirect('/login') 

    
    db = get_db()
    db.row_factory = make_dicts
    if session['type']:
        marks = query_db('SELECT * FROM Mark')
        db.close()

        return render_template('all_marks.html', username=session['username'], marks=marks, admin=session['type'])

@app.route('/complete')
def complete():
    if 'username' not in session:
        return redirect('/login')
    if not session['type']:
        return redirect('/')
    if not request.args:
        return redirect('/remarks')

    mark_id = request.args['markid']

    db = get_db()
    db.row_factory = make_dicts

    remarks = query_db('SELECT DISTINCT * FROM Remark, Mark WHERE Remark.markid = Mark.markid')
    
    try:
        operate1_db('INSERT INTO Complete VALUES(?)', [mark_id])
    except:
        return render_template('remarks.html', username = session['username'], remarks = remarks, type = session['type'], error = True)
    
    return render_template('remarks.html', username = session['username'], remarks = remarks, type = session['type'], warning = True)

@app.route('/remark')
def remark():

    if 'username' not in session:
        return redirect('/login')
    if session['type']:
        return redirect('/')
    if not request.args:
        return redirect('/marks')

    text = request.args['text']
    mark_id = request.args['markid']
    db = get_db()
    db.row_factory = make_dicts
    marks = query_db('SELECT * FROM Mark WHERE username=?', [session['username']])

    if len(text) == 0:
        return render_template('marks.html', username = session['username'], marks = marks, type = session['type'], error = True)    
    try:
        operate1_db('INSERT INTO Remark VALUES(?, ?)', (mark_id, text))
    except:
        return redirect('/marks')

    return render_template('marks.html', username = session['username'], marks = marks, type = session['type'], warning = True)

@app.route('/remarks')
def remarks():

    if 'username' not in session:
        return redirect('/login')
    if not session['type']:
        return redirect('/')
    db = get_db()
    db.row_factory = make_dicts
    remarks = query_db(
        'SELECT DISTINCT * FROM Remark, Mark WHERE Remark.markid = Mark.markid')
    db.close()
    return render_template('remarks.html', remarks=remarks)

@app.route('/postmark')
def postmark():
    if 'username' not in session:
        return redirect('/login') 
    if session['type'] == 0:
        return redirect('/')
    if not request.args:
        db = get_db()
        db.row_factory = make_dicts
        students = query_db('SELECT username FROM User WHERE type = 0')
        worknames = query_db('SELECT DISTINCT workname FROM Mark ORDER BY workname asc')
        db.close()
        return render_template('postmark.html', student_names=students, worknames=worknames)
    else:
        if request.args['workname'] == "" or request.args['grade'] == "": 
            return redirect('/postmark')

        workname = request.args['workname']
        grade = float(request.args['grade'])
        username = request.args['username']

        HaveGrade = query_db('SELECT username, workname FROM Mark WHERE username=? AND workname=?',[username, workname], one=True)
        if (HaveGrade):
            operate2_db('DELETE FROM Mark WHERE workname=? AND username=?', [workname, username])
        operate1_db('INSERT INTO Mark (workname, grade, username) VALUES (?, ?, ?)',(workname, grade, username))
        return redirect('/success')

@app.route('/success')
def success():
    if 'username' not in session:
        return redirect('/login') 
    if session['type'] == 0:
        return redirect('/')
    if not request.args:
        db = get_db()
        db.row_factory = make_dicts
        students = query_db('SELECT username FROM User WHERE type = 0')
        worknames = query_db('SELECT DISTINCT workname FROM Mark ORDER BY workname asc')
        db.close()
        return render_template('success.html', student_names=students, worknames=worknames)
    else:
        if request.args['workname'] == "" or request.args['grade'] == "": 
            return redirect('/postmark')

        workname = request.args['workname']
        grade = float(request.args['grade'])
        username = request.args['username']

        HaveGrade = query_db('SELECT username, workname FROM Mark WHERE username=? AND workname=?',[username, workname], one=True)
        if (HaveGrade):
            operate2_db('DELETE FROM Mark WHERE workname=? AND username=?', [workname, username])
        operate1_db('INSERT INTO Mark (workname, grade, username) VALUES (?, ?, ?)',(workname, grade, username))
        return redirect('/success')

@app.route('/feedback')
def feedback(): 
    new_db = get_db()
    new_db.row_factory = make_dicts
    instructor = query_db('SELECT username FROM User WHERE type = 1')

    if 'username' not in session:
        return redirect('/login')
    
    if session['type']:
        return redirect('/')
    
    if request.args:
        new_instructor = request.args['username']
        answer1 = request.args['answer1']
        answer2 = request.args['answer2']
        answer3 = request.args['answer3']
        answer4 = request.args['answer4']
        # empty input for feedback
        condition1 = (answer1 == "" and answer2 == "" and answer3 == "" and answer4 == "")
        # empty input for instructor
        condition2 = (new_instructor == "Select an instructor:")

        if condition1 and condition2:
            return render_template('feedback.html', instructors = instructor, empty=1, noName=1)
        elif condition1:
            return render_template('feedback.html', instructors = instructor, empty=1)
        elif condition2:
            return render_template('feedback.html', instructors = instructor, noName=1)
        else:
            operate1_db('INSERT INTO Feedback VALUES (?, ?, ?, ?, ?)', (new_instructor, answer1, answer2, answer3, answer4))
            return render_template('feedback.html', instructors = instructor, success=1)

    if not request.args:
        return render_template('feedback.html', instructors = instructor, username = session['username'])

@app.route('/feedbacks')
def feedbacks():
    if 'username' not in session:
        return redirect('/login')
    if not session['type']:
        return redirect('/')
    
    db = get_db()
    db.row_factory = make_dicts
    feedbacks = query_db('SELECT * FROM Feedback WHERE instructor=?', [session['username']])
    db.close()

    return render_template('feedbacks.html', feedbacks=feedbacks, username = session['username'])

@app.route('/logout')
def logout():
    session.pop('username')
    return redirect('/login')



if __name__ == "__main__":
    app.run(debug=True ,port=5000,use_reloader=False)
