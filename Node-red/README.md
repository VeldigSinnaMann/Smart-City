Dette er Node-red-mqtt filen. Her finner man eksempelkode for mqtt/node-red kommunikasjon samt all koden til Node-red sin server-side informasjon(nodene på nettsiden). Controller_reciever/sender_example.cpp er eksempelkode for å bruke mqtt til å styre zumoen remote. Node-red er lagt opp til å fungere med de publish/subscribe stringene som er brukt i koden. den er altså klar for bruk.

* rawJsonForNodeRed.json er bare å kopiere hele filen og lime inn i import på Node-red om man ønsker å åpne dette på egen maskin. For å få til kryptering kan du bruke openssl og lage deg sertifikat. du trenger også installere node-red-dashboard.

* bankDiscountDepositum.csv er "kontoen" hvor vi får inn overskuddet av "bøtlegging" for biler som kjører dårlig. disse pengene blir brukt til å gi rabatterte priser til de som kjører bra. kontoen startet med 10,000kr 
Lag en pott. 
De som kjører under grad 3 må betale 5% mer I strømpris dette legges I potten.
Denne poten brukes til de som kjører 10% best. Disse får disse pengene fordelt.

*bomEsp32 er koden til den fysiske bommen. denne sender skiltnummer til skyen og bøtlegger om bilen er ikke elektrisk


*info til bankDetail.csv
depositumskonto for rabatt: 12345678


