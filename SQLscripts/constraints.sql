ALTER TABLE musico ADD CONSTRAINT musico_fk FOREIGN KEY (artista_nome,artista_datainicio) REFERENCES artista(nome,datainicio);

ALTER TABLE grupo ADD CONSTRAINT grupo_fk FOREIGN KEY (artista_nome,artista_datainicio) REFERENCES artista(nome,datainicio);

ALTER TABLE album ADD CONSTRAINT album_fk FOREIGN KEY (editora_nome) REFERENCES editora(nome);

ALTER TABLE criticamusica ADD CONSTRAINT criticamusica_fk1 FOREIGN KEY (musica_id) REFERENCES musica(id);
ALTER TABLE criticamusica ADD CONSTRAINT criticamusica_fk2 FOREIGN KEY (critica_utilizador_nick) REFERENCES utilizador(nick);

ALTER TABLE criticaalbum ADD CONSTRAINT criticaalbum_fk1 FOREIGN KEY (album_id) REFERENCES album(id);
ALTER TABLE criticaalbum ADD CONSTRAINT criticaalbum_fk2 FOREIGN KEY (critica_utilizador_nick) REFERENCES utilizador(nick);

ALTER TABLE playlist ADD CONSTRAINT playlist_fk1 FOREIGN KEY (utilizador_nick) REFERENCES utilizador(nick);

ALTER TABLE ficheiro ADD CONSTRAINT ficheiro_fk1 FOREIGN KEY (utilizador_nick) REFERENCES utilizador(nick);
ALTER TABLE ficheiro ADD CONSTRAINT ficheiro_fk2 FOREIGN KEY (musica_id) REFERENCES musica(id);

ALTER TABLE notificacoes ADD CONSTRAINT notificacoes_fk1 FOREIGN KEY (utilizador_nick) REFERENCES utilizador(nick);

ALTER TABLE musicogrupo ADD CONSTRAINT musicogrupo_fk1 FOREIGN KEY (grupo_artista_nome,grupo_artista_datainicio) REFERENCES grupo(artista_nome,artista_datainicio);
ALTER TABLE musicogrupo ADD CONSTRAINT musicogrupo_fk2 FOREIGN KEY (musico_artista_nome,musico_artista_datainicio) REFERENCES musico(artista_nome,artista_datainicio);

ALTER TABLE registoacesso ADD CONSTRAINT registoacesso_fk1 FOREIGN KEY (utilizador_nick) REFERENCES utilizador(nick);

ALTER TABLE utilizador_ficheiro ADD CONSTRAINT utilizador_ficheiro_fk1 FOREIGN KEY (utilizador_nick) REFERENCES utilizador(nick);
ALTER TABLE utilizador_ficheiro ADD CONSTRAINT utilizador_ficheiro_fk2 FOREIGN KEY (ficheiro_utilizador_nick,ficheiro_musica_id) REFERENCES ficheiro(utilizador_nick,musica_id);

ALTER TABLE utilizador_playlist ADD CONSTRAINT utilizador_playlist_fk1 FOREIGN KEY (utilizador_nick) REFERENCES utilizador(nick);
ALTER TABLE utilizador_playlist ADD CONSTRAINT utilizador_playlist_fk2 FOREIGN KEY (playlist_nome,playlist_utilizador_nick) REFERENCES playlist(nome,utilizador_nick);

ALTER TABLE musica_genero ADD CONSTRAINT musica_genero_fk1 FOREIGN KEY (musica_id) REFERENCES musica(id);
ALTER TABLE musica_genero ADD CONSTRAINT musica_genero_fk2 FOREIGN KEY (genero_tipo) REFERENCES genero(tipo);

ALTER TABLE playlist_musica ADD CONSTRAINT playlist_musica_fk1 FOREIGN KEY (playlist_nome,playlist_utilizador_nick) REFERENCES playlist(nome,utilizador_nick);
ALTER TABLE playlist_musica ADD CONSTRAINT playlist_musica_fk2 FOREIGN KEY (musica_id) REFERENCES musica(id);

ALTER TABLE artista_concertos ADD CONSTRAINT artista_concertos_fk1 FOREIGN KEY (artista_nome,artista_datainicio) REFERENCES artista(nome,datainicio);
ALTER TABLE artista_concertos ADD CONSTRAINT artista_concertos_fk2 FOREIGN KEY (concertos_nome,concertos_data) REFERENCES concertos(nome,data);

ALTER TABLE musica_album ADD CONSTRAINT musica_album_fk1 FOREIGN KEY (musica_id) REFERENCES musica(id);
ALTER TABLE musica_album ADD CONSTRAINT musica_album_fk2 FOREIGN KEY (album_id) REFERENCES album(id);

ALTER TABLE musica_musico ADD CONSTRAINT musica_musico_fk1 FOREIGN KEY (musica_id) REFERENCES musica(id);
ALTER TABLE musica_musico ADD CONSTRAINT musica_musico_fk2 FOREIGN KEY (musico_artista_nome,musico_artista_datainicio) REFERENCES musico(artista_nome,artista_datainicio);

ALTER TABLE musica_artista ADD CONSTRAINT musica_artista_fk1 FOREIGN KEY (musica_id) REFERENCES musica(id);
ALTER TABLE musica_artista ADD CONSTRAINT musica_artista_fk2 FOREIGN KEY (artista_nome,artista_datainicio) REFERENCES artista(nome,datainicio);