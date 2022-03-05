<?php

    function checkIPAddr($ip){
        $flag = false;

        if (preg_match('/^(?:25[0-5]|2[0-4]\d|1\d\d|[1-9]\d|\d)(?:[.](?:25[0-5]|2[0-4]\d|1\d\d|[1-9]\d|\d)){3}$/',$ip)) { 
            $flag = true; 
        } 
        
        return $flag;

    }

    header("Content-Type: application/json;charset=utf-8");

    $domain = $_POST['domain'];
    $type = $_POST['type'];
    
    

    $ip = gethostbyname($domain);

    $ch = curl_init();
    $dataFormat = "json";
    $key = "2021072721333667603820";
    $url = "http://whois.kisa.or.kr/openapi/whois.jsp?query=".$domain."&key=".$key."&answer=".$dataFormat."";

    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

    
    $output = curl_exec($ch);
    $curl_errno = curl_errno($ch);
    $curl_error = curl_error($ch);

    $output = preg_replace('/\n/','',$output);
    $whois = json_decode($output, true);

    curl_close($ch);

    if($type=="mx"){
        $nslookup = dns_get_record($domain, DNS_MX);
    }
    else {
        $nslookup = dns_get_record($domain, DNS_NS);
    }


    echo(json_encode(array("whois"=>$whois, "nslookup"=>$nslookup, "ip"=>$ip)));

    //echo $_SERVER['SERVER_NAME'];
    exit();
?>
