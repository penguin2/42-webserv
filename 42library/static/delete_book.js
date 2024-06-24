document.addEventListener("DOMContentLoaded", function () {
	const link = document.getElementById("delete_book");

	link.addEventListener("click", function (event) {
		event.preventDefault();

		const cgi_url = "/42library/book_loan.py?book_id=" + link.getAttribute("book_id") + "&control=delete";
		const book_image_url = "/images/" + link.getAttribute("book_id") + ".png";

		sendRequest(cgi_url, "GET", function (status) {
			if (status === 200) {
				sendRequest(book_image_url, "DELETE", function (status) {
					if (status === 204) {
						window.location.href = "/42library/index.py";
					}
				});
			} else {
				alert("Error Transaction1: " + status);
			}
		});
	});
});

function sendRequest(path, method, callback) {
	const xhr = new XMLHttpRequest();
	xhr.open(method, path, true);

	xhr.onreadystatechange = function () {
		if (xhr.readyState === 4) {
			callback(xhr.status);
		}
	};
	xhr.send();
}
