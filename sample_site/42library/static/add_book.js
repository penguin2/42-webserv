const bookImage = document.getElementById('book_image')
const showBookImage = document.getElementById('show_book_image')
const addBookForm = document.getElementById('add_book_form');
const requestErrorMessage = document.getElementById('request_error_message');

// 画像を選択後に選択した画像を表示させるため
bookImage.addEventListener('change', function (event) {
	const file = event.target.files[0]
	if (file) {
		const render = new FileReader()
		render.onload = function (e) {
			showBookImage.innerHTML = '<img src="' + e.target.result + '" style="max-width:200px; height:auto; margin-top: 10px;">';
		};
		render.readAsDataURL(file);
	}
	else {
		showBookImage.innerHTML = '';
	}
});

// add book
addBookForm.addEventListener('submit', function (event) {
	event.preventDefault();

	const formData = new FormData();
	formData.append('title', document.getElementById('title').value);
	formData.append('max_loan_sec', document.getElementById('max_loan_sec').value);
	formData.append('isbn', document.getElementById('isbn').value);
	const file = bookImage.files[0];
	formData.append('book_image', file);

	const cgi_url = "/42library/try_add_book.py";

	sendRequest(cgi_url, "POST", formData, function (status) {
		if (status == 201) {
			window.location.href = "/42library/index.py";
		} else {
			requestErrorMessage.innerHTML = "<p>Error Add Book</p>"
		}
	})
})

function sendRequest(path, method, body, callback) {
	const xhr = new XMLHttpRequest();
	xhr.open(method, path, true);

	xhr.onreadystatechange = function () {
		if (xhr.readyState === 4) {
			callback(xhr.status);
		}
	};
	xhr.send(body);
}
