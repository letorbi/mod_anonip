mod_anonip
==========

A simple Apache2 module to anonymize the last 2 bytes of any received IP address.

The original version of mod_anonip is based on [mod_removeip](http://www.wirspeichernnicht.de/content/view/14/24/),
but mod_anonip just anonymizes the IP-address by removing the last two bytes instead of removing the IP-adress completely.
Unlike mod_removeip mod_anonip allows you to log the origin countries of requests and to serve website content based on GeoIP,
but is still in accordance with the privacy recommendations of the [Independent Centre for Privacy Protection in Germany (ULD)](https://www.datenschutzzentrum.de).
