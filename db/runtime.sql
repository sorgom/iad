CREATE TABLE OnlineState
(
    Id INTEGER PRIMARY KEY AUTOINCREMENT,
	Name TEXT, 
	Rate INTEGER
);
INSERT INTO OnlineState VALUES(1, 'LAN Tx Packets', 111);
INSERT INTO OnlineState VALUES(2, 'LAN Rx Packets', 222);
INSERT INTO OnlineState VALUES(3, 'WAN Rx Packets', 333);
INSERT INTO OnlineState VALUES(4, 'WAN Tx Packets', 444);
INSERT INTO OnlineState VALUES(5, 'WAN Rx Rate', 555);
INSERT INTO OnlineState VALUES(6, 'WAN Tx Rate', 666);
INSERT INTO OnlineState VALUES(7, 'DSL Rx Cells', 777);
INSERT INTO OnlineState VALUES(8, 'DSL Tx Cells', 888);
