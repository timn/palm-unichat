<?php

/* $Id: index.php,v 1.2 2003/11/04 15:01:08 tim Exp $ */

$title="niemueller.de - palm software - UniChat";
$meta_desc="PalmOS software for digital wireless whispering";
$meta_keyw="Palm, PalmOS, university, chat, whisper, infrared, ir, irda";
include("$DOCUMENT_ROOT/header.inc.php");
include("$DOCUMENT_ROOT/left.inc.php");

$version="0.3";
$basename="UniChat";
$languages=array( "de" => "German/Deutsch",
                  "en" => "English"
                );

heading_lastmod();
heading("Uni Chat",
        "PalmOS software for digital wireless whispering", "unichat_big.png");
?>
<div class="maintext">
<h5>Introduction</h5>
This PalmOS applications allows you to chat in absolutely noiseless environments. OK, a room
full of students "listening" to a lecture is never quiet... But if sitting several seets separated
this software has proven to be useful for conversation.
<br/>
Some features:
<ul>
 <li>Beam messages between palm fast and without any "Do you want to accept..." questions</li>
 <li>Nice start message ;-)</li>
</ul>

<br/>
Just to be sure that you heard: You will need a PalmOS device to run this
application. You can also use the Palm OS Emulator
<a href="http://www.palmos.com/dev/tools/emulator/" rel="external">POSE</a>.
Linux binaries can be found
<a href="http://sourceforge.net/project/showfiles.php?group_id=2189" rel="external">here</a>.
<br/>
<i>Do not ask for ROM files</i>. I cannot and I do not want to share
my ROMs with you. Go to the mentioned POSE page, there is a description
on how to obtain ROM files from Palm Source directly and legally
without owning a Palm handheld.
<br/><br/>
UniChat is available in multiple languages. Supported languages are right now (randomly ordered):
<ul>
 <li>German (Deutsch)</li>
 <li>English</li>
</ul>
If you want to help and translate UniMensa to another language please
<a href="mailto:tim@niemueller.de?Subject=Translate UniChat">contact</a> me and I will
send you the language file.


<h5>News</h5>
<b>2002/11/28</b><br/>
First public release.
If you find any bugs please send me a <a href="mailto:unichat-bug@niemueller.de">mail</a> describing the
problem (please note OS and hardware you use).
<b>2003/01/30</b><br/>
Fixed odd bug that caused PalmOS 3.5.x (and probably earlier) handhelds to crash on menu access. Seems that there
is a BadBug(TM) in the MenuHideItem function...


<h5>Thanks</h5>
A special thanks to my friends Tobias "The Bugfinder" Weyand and Sumedha Ananda Markus Widyadharma (just had to write
whole name :-) who greatly helped to reduce my Bug rate in this
software (and others...). All remaining bugs are my fault. I probably inserted them after their checks...


<h5>Screenshots</h5>
Some screenshots will give you an impression of the software.
<br/><br/>
<table border="0" width="100%" cellpadding="0">
 <tr>
  <td align="center" style="width:33%;"><img src="screens/main.png" style="border:1px solid #c0c0c0;" alt="Main screen" /></td>
  <td align="center" style="width:33%;"><img src="screens/about.png" style="border:1px solid #c0c0c0;" alt="Edit Time" /></td>
 </tr>
 <tr>
  <th>Main screen</th>
  <th>About Screen</th>
 </tr>
</table>

<h5>Supported devices</h5>
Any PalmOS handheld device running PalmOS 3.5 or higher should work for UniChat.
<br/><br/>I have tested UniChat on:<br/>
Palm m515 (PalmOS 4.1, color, tested on real device, works perfectly)<br/>
Palm IIIc (PalmOS 3.5, color, tested on real device, works perfectly)<br/>
Palm IIIx (PalmOS 4.0, gray, tested on real device, works perfectly)<br/>
Palm IIIx (PalmOS 3.5, gray, tested on emulator, works perfectly)<br/>

<h5>Download</h5>
UniChat is released and distributed under the terms of a proprietry
license. You may use the software for free and beam it to your friend.
You may not distribute it under your name. You may distribute it on
so called "Shareware or Freeware CD-ROMs". You must then inform the author
<i>before</i> the CDs are published. The author has the right to deny distribution
of the software without further need of explanation.
<br/>
<span style="color: red;">By downloading and/or using the software you agree
to this license!</span>
<br/><br/>
You may download the Palm install file provided as a .zip and as a .tar.gz file.
<br/>Latest <b>stable</b> version is <span style="color:red;"><?=$version?></span>.
<br/><br/>
<table border="0" cellpadding="0" cellspacing="0">
<? foreach ($languages as $key => $value) { ?>
<?   if (file_exists("{$basename}-{$version}_dist/{$basename}-{$version}-{$key}.tar.gz")) { ?>
 <tr>
  <td><?=$basename?> <?=$version?> (<?=$value?>)</td>
  <td>&nbsp; &nbsp;</td>
  <td><a href="<?=$basename?>-<?=$version?>_dist/<?=$basename?>-<?=$version?>-<?=$key?>.tar.gz">.tar.gz</a> (<?=filesize("{$basename}-{$version}_dist/{$basename}-{$version}-{$key}.tar.gz")?> Bytes)</td>
  <td>&nbsp;</td>
  <td><a href="<?=$basename?>-<?=$version?>_dist/<?=$basename?>-<?=$version?>-<?=$key?>.zip">.zip</a> (<?=filesize("{$basename}-{$version}_dist/{$basename}-{$version}-{$key}.zip")?> Bytes)</td>
 </tr>
<?   } ?>
<? } ?>
 </tr>
</table>
<br/>

</div>

<? include("$DOCUMENT_ROOT/right.inc.php"); ?>
<? include("$DOCUMENT_ROOT/footer.inc.php"); ?>
