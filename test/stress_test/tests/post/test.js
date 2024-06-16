import http from 'k6/http';
import { sleep } from 'k6';
import { check } from 'k6';

export const options = {
    vus: 10,
	duration: '10s',
	rps: 10,
};

function generateUniqueId() {
    const randomString = Math.random().toString(36).slice(2, 11);
    return randomString;
}

const data = JSON.stringify({ largeField: 'x'.repeat(10 * 1024 * 1024 - 20) });
let counter = 0;

export default function () {
	const uniqueId = generateUniqueId();
	const fileName = `file_${uniqueId}.json`;

    const res = http.post(`http://localhost:4242/upload/${fileName}`, data, {
        headers: { 'Content-Type': 'application/json' },
    });

    check(res, {
        'is status 201': (r) => r.status === 201,
    });

    sleep(1);
}

