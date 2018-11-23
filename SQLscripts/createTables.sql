CREATE TABLE musica (
	id	 BIGINT,
	nome	 VARCHAR(50) NOT NULL,
	data	 TIME NOT NULL,
	duracao DATE NOT NULL,
	letra	 VARCHAR(2048),
	PRIMARY KEY(id)
);

CREATE TABLE interprete (
	nome	 VARCHAR(512),
	datainicio DATE,
	datafim	 DATE,
	historia	 VARCHAR(512),
	PRIMARY KEY(nome,datainicio)
);

CREATE TABLE musico (
	genero		 VARCHAR(512) NOT NULL,
	citacao		 VARCHAR(512),
	localnascimento	 VARCHAR(512),
	localmorte		 VARCHAR(512),
	nomenascenca		 VARCHAR(512),
	interprete_nome	 VARCHAR(512),
	interprete_datainicio DATE,
	PRIMARY KEY(interprete_nome,interprete_datainicio)
);

CREATE TABLE grupo (
	interprete_nome	 VARCHAR(512),
	interprete_datainicio DATE,
	PRIMARY KEY(interprete_nome,interprete_datainicio)
);

CREATE TABLE album (
	id		 BIGINT,
	nome		 VARCHAR(50) NOT NULL,
	datalancamento DATE NOT NULL,
	detalhes	 VARCHAR(512),
	tipo		 VARCHAR(20),
	capaimagem	 BYTEA,
	editora_nome	 BIGINT NOT NULL,
	PRIMARY KEY(id)
);

CREATE TABLE genero (
	tipo	 VARCHAR(20),
	descricao VARCHAR(512),
	PRIMARY KEY(tipo)
);

CREATE TABLE utilizador (
	nick		 VARCHAR(20),
	password	 VARCHAR(20) NOT NULL,
	ultimonome	 VARCHAR(50) NOT NULL,
	primeironome	 VARCHAR(50) NOT NULL,
	email		 VARCHAR(50) UNIQUE NOT NULL,
	dataregisto	 DATE NOT NULL,
	gestor	 BOOL NOT NULL,
	mailconfirmado BOOL NOT NULL,
	datanascimento DATE,
	bio		 VARCHAR(512),
	pais		 VARCHAR(50),
	PRIMARY KEY(nick)
);

CREATE TABLE criticamusica (
	musica_id		 BIGINT NOT NULL,
	critica_data		 DATE,
	critica_pontuacao	 SMALLINT NOT NULL DEFAULT 0,
	critica_justificacao	 VARCHAR(512),
	critica_utilizador_nick VARCHAR(20),
	PRIMARY KEY(critica_data,critica_utilizador_nick)
);

CREATE TABLE criticaalbum (
	album_id		 BIGINT NOT NULL,
	critica_data		 DATE,
	critica_pontuacao	 SMALLINT NOT NULL DEFAULT 0,
	critica_justificacao	 VARCHAR(512),
	critica_utilizador_nick VARCHAR(20),
	PRIMARY KEY(critica_data,critica_utilizador_nick)
);

CREATE TABLE editora (
	nome	 BIGINT,
	datafundacao DATE NOT NULL,
	detalhes	 VARCHAR(512),
	pais	 VARCHAR(50),
	webpage	 VARCHAR(50) UNIQUE,
	PRIMARY KEY(nome)
);

CREATE TABLE concertos (
	nome	 VARCHAR(512),
	data	 DATE,
	informacoes VARCHAR(1024) NOT NULL,
	localizacao VARCHAR(512) NOT NULL,
	descricao	 VARCHAR(1024),
	webpage	 VARCHAR(50),
	PRIMARY KEY(nome,data)
);

CREATE TABLE playlist (
	nome		 VARCHAR(30),
	data		 DATE NOT NULL,
	private	 BOOL NOT NULL,
	descricao	 VARCHAR(512),
	utilizador_nick VARCHAR(20),
	PRIMARY KEY(nome,utilizador_nick)
);

CREATE TABLE ficheiro (
	dataupload	 DATE NOT NULL,
	ficheiro	 BYTEA NOT NULL,
	comentario	 VARCHAR(512),
	utilizador_nick VARCHAR(20),
	musica_id	 BIGINT,
	PRIMARY KEY(utilizador_nick,musica_id)
);

CREATE TABLE notificacoes (
	data		 DATE,
	nome		 VARCHAR(50) NOT NULL,
	importante	 BOOL NOT NULL,
	lida		 BOOL NOT NULL,
	descricao	 VARCHAR(512) NOT NULL,
	utilizador_nick VARCHAR(20),
	PRIMARY KEY(data,utilizador_nick)
);

CREATE TABLE musicogrupo (
	papel			 VARCHAR(512) NOT NULL,
	datainicio			 DATE,
	datafim			 DATE,
	grupo_interprete_nome	 VARCHAR(512),
	grupo_interprete_datainicio	 DATE,
	musico_interprete_nome	 VARCHAR(512),
	musico_interprete_datainicio DATE,
	PRIMARY KEY(grupo_interprete_nome,grupo_interprete_datainicio,musico_interprete_nome,musico_interprete_datainicio)
);

CREATE TABLE registoacesso (
	data		 DATE,
	tipoacesso	 VARCHAR(512) NOT NULL,
	utilizador_nick VARCHAR(20),
	PRIMARY KEY(data,utilizador_nick)
);

CREATE TABLE utilizador_ficheiro (
	utilizador_nick		 VARCHAR(20),
	ficheiro_utilizador_nick VARCHAR(20),
	ficheiro_musica_id	 BIGINT,
	PRIMARY KEY(utilizador_nick,ficheiro_utilizador_nick,ficheiro_musica_id)
);

CREATE TABLE utilizador_playlist (
	utilizador_nick		 VARCHAR(20),
	playlist_nome		 VARCHAR(30),
	playlist_utilizador_nick VARCHAR(20),
	PRIMARY KEY(utilizador_nick,playlist_nome,playlist_utilizador_nick)
);

CREATE TABLE musica_genero (
	musica_id	 BIGINT,
	genero_tipo VARCHAR(20),
	PRIMARY KEY(musica_id,genero_tipo)
);

CREATE TABLE playlist_musica (
	playlist_nome		 VARCHAR(30),
	playlist_utilizador_nick VARCHAR(20),
	musica_id		 BIGINT,
	PRIMARY KEY(playlist_nome,playlist_utilizador_nick,musica_id)
);

CREATE TABLE interprete_concertos (
	interprete_nome	 VARCHAR(512),
	interprete_datainicio DATE,
	concertos_nome	 VARCHAR(512),
	concertos_data	 DATE,
	PRIMARY KEY(interprete_nome,interprete_datainicio,concertos_nome,concertos_data)
);

CREATE TABLE musica_album (
	musica_id BIGINT,
	album_id	 BIGINT NOT NULL,
	PRIMARY KEY(musica_id)
);

CREATE TABLE musica_musico (
	musica_id			 BIGINT,
	musico_interprete_nome	 VARCHAR(512),
	musico_interprete_datainicio DATE,
	PRIMARY KEY(musica_id,musico_interprete_nome,musico_interprete_datainicio)
);

CREATE TABLE musica_interprete (
	musica_id		 BIGINT,
	interprete_nome	 VARCHAR(512),
	interprete_datainicio DATE,
	PRIMARY KEY(musica_id,interprete_nome,interprete_datainicio)
);