/* table listing not resizeable tables */
CREATE TABLE S_NRS
(
	Name TEXT PRIMARY KEY
);

/*	formatting of table fields */
CREATE TABLE S_FRM
(
	Name TEXT,
	Col TEXT,
	Frm TEXT
);

/* single values integer table */
CREATE TABLE SVI
(
    Id INTEGER PRIMARY KEY AUTOINCREMENT,
	Name TEXT,
	Num INTEGER
);
INSERT INTO SVI VALUES(1, 'TimeDiff', NULL);

/*	table is not resizeable */
INSERT INTO S_NRS VALUES('SVI');
/*	field Name is not editable */
INSERT INTO S_FRM  VALUES('SVI', 'Name', 'NOED');


/* general single value bool storage */
CREATE TABLE SVB
(
    Id INTEGER PRIMARY KEY AUTOINCREMENT,
	Name TEXT,
	Ok INTEGER
);
INSERT INTO SVB VALUES(1, 'MacFlt', NULL);
INSERT INTO SVB VALUES(2, 'WLAN', 1);

/*	table is not resizeable */
INSERT INTO S_NRS VALUES('SVB');
/*	field "Name" is not editable */
INSERT INTO S_FRM VALUES('SVB', 'Name', 'NOED');

/* join sample table 1 */
CREATE TABLE ZipNum
(
    Id INTEGER PRIMARY KEY AUTOINCREMENT,
	Country TEXT,
	Area TEXT,
	Nr TEXT
);

INSERT INTO ZipNum VALUES(1, '49', '30', '332332');
INSERT INTO ZipNum VALUES(2, '49', '30', '3451890');
INSERT INTO ZipNum VALUES(3, '49', '30', '4484460');
INSERT INTO ZipNum VALUES(4, '49', '30', '29007217');
INSERT INTO ZipNum VALUES(5, '49', '30', '3256317');

INSERT INTO S_FRM VALUES('ZipNum', '*', 'UNUM');


/* join sample table 2 */
CREATE TABLE NetProv
(
    Id INTEGER PRIMARY KEY AUTOINCREMENT,
	Name TEXT
);

INSERT INTO NetProv VALUES(1, 'Telekom');
INSERT INTO NetProv VALUES(2, 'CIA');

/* join sample table 3 */
CREATE TABLE ZipProv
(
    Id INTEGER PRIMARY KEY AUTOINCREMENT,
	Name TEXT,
	Code TEXT,
	NetProv INTEGER REFERENCES NetProv(Id)
);

INSERT INTO ZipProv VALUES(1, 'Actrice AG', 'A13-4', 1);
INSERT INTO ZipProv VALUES(2, 'Alcor AG', 'ZZZ', 1);
INSERT INTO ZipProv VALUES(3, 'Saphiron AG', 'K', 1);
INSERT INTO ZipProv VALUES(4, 'MobileCom AG', 'LLL', 2);

/* join sample table 4 */
CREATE TABLE ZipAcc
(
    Id INTEGER PRIMARY KEY AUTOINCREMENT,
	Name TEXT,
	ZipProv INTEGER REFERENCES ZipProv(Id),
	ZipNum INTEGER REFERENCES ZipNum(Id)
);

INSERT INTO ZipAcc VALUES(1, 'Actrice privat', 1, 5);
INSERT INTO ZipAcc VALUES(2, 'Actrice Büro', 1, 4);
INSERT INTO ZipAcc VALUES(3, 'Alcor', 2, 3);
INSERT INTO ZipAcc VALUES(4, 'Bude', 3, 2);
INSERT INTO ZipAcc VALUES(5, 'Mogel', 4, NULL);

/*	time servers (host name or ip) table */
CREATE TABLE TimeSrv
(
    Id INTEGER PRIMARY KEY AUTOINCREMENT,
	Name TEXT,
	Fixed INTEGER,
	Active INTEGER
);
INSERT INTO TimeSrv(Name, Fixed, Active) VALUES('europe.pool.ntp.org', 1, 1);
INSERT INTO TimeSrv(Name, Fixed, Active) VALUES('ntp.wumpel.org', 1, NULL);
INSERT INTO TimeSrv(Name, Fixed, Active) VALUES(NULL, NULL, NULL);
INSERT INTO TimeSrv(Name, Fixed, Active) VALUES(NULL, NULL, NULL);
INSERT INTO TimeSrv(Name, Fixed, Active) VALUES(NULL, NULL, NULL);
INSERT INTO TimeSrv(Name, Fixed, Active) VALUES(NULL, NULL, NULL);

INSERT INTO S_FRM VALUES('TimeSrv', 'Name', 'HOST');

/* Real Sample Table */
CREATE TABLE SpREAL
(
    Id INTEGER PRIMARY KEY AUTOINCREMENT,
	Val REAL
);
INSERT INTO SpREAL VALUES(1, 42.000125);
INSERT INTO SpREAL VALUES(2, NULL);
INSERT INTO SpREAL VALUES(3, NULL);
INSERT INTO SpREAL VALUES(4, NULL);
INSERT INTO SpREAL VALUES(5, NULL);

INSERT INTO S_FRM VALUES('SpREAL', '*', 'REAL');

/* Ip4 Sample Table */
CREATE TABLE SpIP4
(
    Id INTEGER PRIMARY KEY AUTOINCREMENT,
	Val TEXT
);

INSERT INTO SpIP4 VALUES(1, '192.168.1.10');
INSERT INTO SpIP4 VALUES(2, NULL);
INSERT INTO SpIP4 VALUES(3, NULL);
INSERT INTO SpIP4 VALUES(4, NULL);
INSERT INTO SpIP4 VALUES(5, NULL);

INSERT INTO S_FRM VALUES('SpIP4', '*', 'IP4');

/* Mac Sample Table */
CREATE TABLE SpMAC
(
    Id INTEGER PRIMARY KEY AUTOINCREMENT,
	Val TEXT
);
INSERT INTO SpMAC VALUES(1, '01:02:03:0A:0B:0C');
INSERT INTO SpMAC VALUES(2, NULL);
INSERT INTO SpMAC VALUES(3, NULL);
INSERT INTO SpMAC VALUES(4, NULL);
INSERT INTO SpMAC VALUES(5, NULL);

INSERT INTO S_FRM VALUES('SpMAC', '*', 'MAC');

/* Host Sample Table */
CREATE TABLE SpHOST
(
    Id INTEGER PRIMARY KEY AUTOINCREMENT,
	Val TEXT
);
INSERT INTO SpHOST VALUES(1, '192.168.1.10');
INSERT INTO SpHOST VALUES(2, 'sorgo.de');
INSERT INTO SpHOST VALUES(3, NULL);
INSERT INTO SpHOST VALUES(4, NULL);
INSERT INTO SpHOST VALUES(5, NULL);

INSERT INTO S_FRM VALUES('SpHOST', '*', 'HOST');

/* Text Sample Table */
CREATE TABLE SpTEXT
(
    Id INTEGER PRIMARY KEY AUTOINCREMENT,
	Val TEXT
);
INSERT INTO SpTEXT VALUES(1, 'Notebook Heinz'); 
INSERT INTO SpTEXT VALUES(2, 'Lola''s "Macbook"'); 
INSERT INTO SpTEXT VALUES(3, "PC 1st Floor");
INSERT INTO SpTEXT VALUES(4, "Geier & Habicht GmbH");
INSERT INTO SpTEXT VALUES(5, NULL);

/*	WLAN MAC Adrress Filter Table */
CREATE TABLE MacFlt
(
    Id INTEGER PRIMARY KEY AUTOINCREMENT,
	Name TEXT,
	Mac TEXT,
	Active INTEGER
);
INSERT INTO MacFlt VALUES(1, 'Notebook Heinz', 'FF:A3:03:04:AA:06', 1); 
INSERT INTO MacFlt VALUES(2, 'Lola''s Macbook', '01:02:03:04:05:06', 1); 
INSERT INTO MacFlt VALUES(3, "John's PC 1st Floor", 'A4:02:03:04:05:06', NULL); 

/* No Data test table */
CREATE TABLE NoData
(
	Id INTEGER PRIMARY KEY AUTOINCREMENT,
	Name TEXT,
	Active INTEGER
);

INSERT INTO S_NRS VALUES('NoData');

/* 
	no resize tables from other DBs. 
	yes, here someone has to know what the setup is.
*/
INSERT INTO S_NRS VALUES('OnlineState');

/* 
	field formats of tables from other DBs. 
	yes, here someone has to know what the setup is.
*/
INSERT INTO S_FRM VALUES('OnlineState', 'Name', 'NOED');
INSERT INTO S_FRM VALUES('OnlineState', 'Rate', 'NUM');

