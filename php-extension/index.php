<?php declare(strict_types=1); ini_set('display_errors', '1'); ini_set('display_startup_errors', '1'); error_reporting(E_ALL); assert_options(ASSERT_ACTIVE, 1); assert_options(ASSERT_WARNING, 1); ?>
<?php

    echo http_normalize('https://www.youtube.com:90/../x/./?b=x&a=d'), "\n";
    echo http_normalize('https://www.youtube.com:90/'), "\n";
    echo http_normalize('https://www.youtube.com:90/xyz/'), "\n";
    echo http_normalize('https://www.youtube.com:90/xyz?z=&x&y'), "\n";

    echo http_normalize_to_abs('google.com/sadlife/?x=y', 'https://dUckduckgo.com', '/s%21%2f/?q=hsad&t=ffab&ia=web'), "\n";
    echo http_normalize_to_abs('/', 'https://dUckduckgo.com', '/s%21%2f/?q=hsad&t=ffab&ia=web'), "\n";
?>
