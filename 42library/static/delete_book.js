document.addEventListener("DOMContentLoaded", function () {
	const link = document.getElementById("delete_book");

	// id="delete_book"が付与されたhtml要素がClickされた時にイベント発生
	link.addEventListener("click", function (event) {
		// イベント発生後にページ推移しない
		event.preventDefault();

		const book_id = link.getAttribute("book_id")
		// CGIスクリプトによるDBからの図書データの削除用URL
		const cgi_url = "/42library/book_loan.py?book_id=" + book_id + "&control=delete";
		// 静的なDELETEメソッドでファイルを削除する用のURL
		const book_image_url = "/images/" + book_id + ".png";

		sendRequest(cgi_url, "GET", function (status) {
			if (status === 200) {
				// 図書データの削除が成功したらファイル削除のDELETEリクエスト
				sendRequest(book_image_url, "DELETE", function (status) {
					window.location.href = "/42library/index.py";
				});
			} else {
				// 図書データの削除に失敗したらアラートを出す
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
