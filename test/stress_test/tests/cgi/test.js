import http from 'k6/http';
import { sleep } from 'k6';
import { check } from 'k6';

export const options = {
    vus: 10,
    duration: '5s',
    rps: 10,
};

export default function () {

    const res = http.get('http://localhost:4242/cgi.sh');

    check(res, {
        'is status 200': (r) => r.status === 200,
    });

    sleep(1);
}

