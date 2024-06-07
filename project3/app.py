from flask import Flask, request, send_from_directory, render_template, redirect, url_for
import os

app = Flask(__name__)


UPLOAD_FOLDER = '/mnt/uploads'
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

os.makedirs(UPLOAD_FOLDER, exist_ok=True)

@app.route('/')
def index():
    files = os.listdir(app.config['UPLOAD_FOLDER'])
    return render_template('index.html', files=files)

@app.route('/upload', methods=['POST'])
def upload_file():

    required_password = "root"

    if 'password' not in request.form or request.form['password'] != required_password:
        return "Authentication failed", 403
    if 'file' not in request.files:
        return redirect(request.url)

    file = request.files['file']

    if file.filename == '':
        return redirect(request.url)

    if file:
        file.save(os.path.join(app.config['UPLOAD_FOLDER'], file.filename))
    
    return redirect(url_for('index'))


@app.route('/delete/<filename>', methods=['GET'])
def delete_file(filename):
    required_password = "root"

    if 'password' not in request.args or request.args['password'] != required_password:
        return "Authentication failed", 403

    file_path = os.path.join(app.config['UPLOAD_FOLDER'], filename)
    if os.path.exists(file_path):
        os.remove(file_path)
        return redirect(url_for('index'))
    else:
        return "File not found", 404

@app.route('/download/<filename>')
def download(filename):
    return send_from_directory(app.config['UPLOAD_FOLDER'], filename)

if __name__ == '__main__':
     app.run(debug=True, host='0.0.0.0', port=5000)