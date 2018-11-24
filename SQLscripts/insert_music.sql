INSERT INTO musica (id, nome, data, duracao, letra)
		Values (1, 'Bad Liar', '2018-01-01', '00:04:21','Oh, hush, my dear, its been a difficult year
And terrors don t prey on innocent victims
Trust me, darlin, trust me darlin
Its been a loveless year
Im a man of three fears
Integrity, faith and crocodile tears
Trust me, darlin, trust me, darlin
So look me in the eyes, tell me what you see
Perfect paradise, tearin at the seams
I wish I could escape, I dont wanna fake it
Wish I could erase it, make your heart believe
But Im a bad liar, bad liar
Now you know, now you know
That Im a bad liar, bad liar
Now you know, you’re free to go
Did all my dreams never mean one thing?
Does happiness lie in a diamond ring?
Oh, ive been askin for
Oh, ive been askin for problems, problems, problems
I wage my war, on the world inside
I take my gun to the enemys side
Oh, Ive been askin for (trust me, darlin)
Oh, Ive been askin for (trust me, darlin) problems, problems, problems
So look me in the eyes, tell me what you see
Perfect paradise, tearin at the seams
I wish I could escape, I dont wanna fake it
Wish I could erase it, make your heart believe
But Im a bad liar, bad liar
Now you know, now you know
That Im a bad liar, bad liar
Now you know, youre free to go
I cant breathe, I cant be
I cant be what you want me to be
Believe me, this one time
Believe me
Im a bad liar, bad liar
Now you know, now you know
That Im a bad liar, bad liar
Now you know, youre free to go
Oh
Please believe me
Please believe me');



-- BAD LIAR SELENA GOMEZ
INSERT INTO musica (id, nome, data, duracao, letra)
		Values (2, 'Bad Liar', '2017-00-00', '00:03:52', 'I was walking down the street the other day
Tryna distract myself
But then I see your face
Oh wait, thats someone else
Ohh, tryna play it coy
Tryna make it disappear
But just like the battle of Troy
Theres nothing subtle here
In my room theres a king size space
Bigger than it used to be
If you want you can rent that place
Call me an amenity
Even if its in my dreams
Ooh youre taking up a fraction of my mind
Ooh every time I watch you serpentine
Oh Im tryin, Im tryin, Im tryin
Im tryin, Im tryin
Oh tryin, Im tryin, Im tryin
Im tryin, Im tryin
Not to think about you
No, no, no, no
Not to think about you
No, no, no, no
Oh Im tryin, Im tryin, Im tryin
Im tryin, Im tryin
Oh tryin, Im tryin, Im tryin
Im tryin, Im tryin
Not to give in to you
No, no, no, no
Not to give in to you
With my feelings on fire
Guess Im a bad liar
I see how your attention builds
Its like looking in a mirror
Your touch like a happy pill
But still all we do is fear
What could possibly happen next?
Can we focus on the love?
Paint my kiss across your chest
If youre the art, Ill be the brush
Ooh youre taking up a fraction of my mind
Ooh every time I watch you serpentine
Oh Im tryin Im tryin, Im tryin
Im tryin, Im tryin
Oh tryin, Im tryin, Im tryin
Im tryin, Im tryin
Not to think about you
No, no, no, no
Not to think about you
No, no, no, no
Oh Im tryin, Im tryin, Im tryin
Im tryin, Im tryin
Oh tryin, Im tryin, Im tryin
Im tryin, Im tryin
Not to give in to you
No, no, no, no
Not to give in to you
All of my feelings on fire
Guess Im a bad liar
And oh baby lets make reality, actuality, a reality
Oh baby lets make reality, actuality, a reality
Oh, oh, Im tryin, Im tryin, Im tryin
Im tryin, Im tryin
Oh tryin, Im tryin, Im tryin
Im tryin, Im tryin
Not to think about you
No, no, no, no
Not to think about you
No, no, no, no
Oh Im tryin, Im tryin, Im tryin
Im tryin, Im tryin
Oh tryin, Im tryin, Im tryin
Im tryin, Im tryin
Not to give in to you
No, no, no, no
Not to give in to you
With my feelings on fire
Guess Im a bad liar');

-- insert musicos
INSERT INTO musico(artista_nome, artista_datainicio, genero)
		VALUES('Selena Gomez', '00-00-00', 'Female');

INSERT INTO musico(artista_nome, artista_datainicio, genero)
		VALUES('Julia Michaels', '00-00-00', 'Female');

INSERT INTO musico(artista_nome, artista_datainicio, genero)
		VALUES('Justin Tranter', '00-00-00', 'Male');

INSERT INTO musico(artista_nome, artista_datainicio, genero)
		VALUES('Ian Kirkpatrick', '00-00-00', 'Male');

-- insert quem compoe
INSERT INTO musica_musico(musica_id, musico_artista_nome, musico_artista_datainicio)
		VALUES (2, 'Selena Gomez', '00-00-00');

INSERT INTO musica_musico(musica_id, musico_artista_nome, musico_artista_datainicio)
		VALUES (2, 'Julia Michaels', '00-00-00');

INSERT INTO musica_musico(musica_id, musico_artista_nome, musico_artista_datainicio)
		VALUES (2, 'Justin Tranter', '00-00-00');

INSERT INTO musica_musico(musica_id, musico_artista_nome, musico_artista_datainicio)
		VALUES (2, 'Ian Kirkpatrick', '00-00-00');

-- ARTISTA
INSERT INTO artista(nome, datainicio)
		VALUES('Selena Gomez', '00-00-00');

-- Musica ARTISTA
INSERT INTO musica_artista(musica_id, artista_nome, artista_datainicio)
		VALUES(2, 'Selena Gomez', '00-00-00');

-- ALBUM
INSERT INTO album(id, nome, datalancamento, editora_nome)
		VALUES(1, 'SG2', '2018-00-00', ' Interscope Records');

-- Ligação ALBUM MUSICA
INSERT INTO musica_album(musica_id, album_id)
		VALUES(2, 1);

-- Genero
INSERT INTO genero(tipo)
		VALUES('POP');


-- GENERO MUSICA
INSERT INTO musica_genero(musica_id, genero_tipo)
		VALUES(2, 'POP');

		