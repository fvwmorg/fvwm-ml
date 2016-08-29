$siteInfo->{'pro-linux'} = {
            'name' => "pro-linux",
            'host' => "pl-forum.de",
            'path' => "/backend/pro-linux.rdf",
            'func' => \&processProLinux,
            # the following string is only used in --info
            'flds' => 'time, title, department, topic, author, url',
          };

         sub processProLinux () {
	return processXml(
		'item',
		{ 'h' => 'title', 'u' => 'link', 'd' => undef },
		sub ($) {
			# this site's rdf does not supply the time, how weird...
			#(gmtime())[5,4,3,2,1,0];
			()
		}, +0,
	);
}

         1;


$siteInfo->{'ard'} = {
            'name' => "ard",
            'host' => "tagesschau.de",
            'path' => "/newsticker.rdf",
            'func' => \&processARD,
            # the following string is only used in --info
            'flds' => 'time, title, department, topic, author, url',
          };

         sub processARD () {
	return processXml(
		'item',
		{ 'h' => 'title', 'u' => 'link', 'd' => undef },
		sub ($) {
			# this site's rdf does not supply the time, how weird...
			#(gmtime())[5,4,3,2,1,0];
			()
		}, +0,
	);
}

         1;
	 
$siteInfo->{'heise'} = {
            'name' => "heise",
            'host' => "www.heise.de",
            'path' => "/newsticker/heise.rdf",
            'func' => \&processHeise,
            # the following string is only used in --info
            'flds' => 'time, title, department, topic, author, url',
          };

         sub processHeise () {
	return processXml(
		'item',
		{ 'h' => 'title', 'u' => 'link', 'd' => undef },
		sub ($) {
			# this site's rdf does not supply the time, how weird...
			#(gmtime())[5,4,3,2,1,0];
			()
		}, +0,
	);
}

         1;	 

