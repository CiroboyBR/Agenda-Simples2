# Agenda-Simples
Agenda feita com C + MYSQL + wxWidgets


Ferramentas utilizadas:

CodeBlocks 13.12 + plugin wxSmith      -      http://www.codeblocks.org/downloads

wxWidgets 3.0   -    https://www.wxwidgets.org/news/2014/10/wxwidgets-3.0.2-released/ 

Mysql Connector C    -   https://dev.mysql.com/downloads/connector/c/


Utilize -std=c99 no compilador!

---------------- TABELA NO MYSQL------------------------------

create table pessoa (id INT primary key auto_increment, nome varchar(75) unique, telefone varchar(20) );


mysql> describe pessoa;

+----------+-------------+------+-----+---------+----------------+

| Field    | Type        | Null | Key | Default | Extra          |

+----------+-------------+------+-----+---------+----------------+

| id       | int(11)     | NO   | PRI | NULL    | auto_increment |

| nome     | varchar(75) | NO   | UNI | NULL    |                |

| telefone | varchar(20) | YES  |     | NULL    |                |

+----------+-------------+------+-----+---------+----------------+

3 rows in set (0.08 sec)
