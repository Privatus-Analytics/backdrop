<?php

/**
 * @file
 * Turns on the Testing module in CI so run-tests.sh can run.
 */

define('BACKDROP_ROOT', '/var/www/html');
chdir(BACKDROP_ROOT);
$_SERVER['HTTP_HOST'] = 'localhost';
$_SERVER['REMOTE_ADDR'] = '127.0.0.1';
$_SERVER['SCRIPT_NAME'] = '/index.php';
$_SERVER['REQUEST_METHOD'] = 'GET';
require_once BACKDROP_ROOT . '/core/includes/bootstrap.inc';
backdrop_bootstrap(BACKDROP_BOOTSTRAP_FULL);
exit(module_enable(array('simpletest')) ? 0 : 1);
