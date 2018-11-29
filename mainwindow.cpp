#include "mainwindow.h"
#include "startwindow.h"
#include "playlisteditor.h"
#include "browsemusicdialog.h"
#include "reviewdialog.h"
#include "ui_mainwindow.h"

#include <QSqlDatabase>
#include <QtSql>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDateTime>
#include <QInputDialog>
#include <QFileDialog>

const QString searchRule ="|";

const QString musicSearchQuery = "SELECT DISTINCT m.id, m.nome, a.nome, mi.interprete_nome "
                                 "FROM musica m, musica_album ma, album a, musica_interprete mi "
                                 "WHERE m.id = ma.musica_id "
                                 "AND a.id = ma.album_id "
                                 "AND m.id = mi.musica_id "
                                 "AND m.nome ~* ?";

const QString albumSearchQuery = "SELECT DISTINCT a.id, a.nome, mi.interprete_nome, to_char(a.datalancamento,'YYYY') "
                                 "FROM musica m, musica_album ma, album a, musica_interprete mi "
                                 "WHERE m.id = ma.musica_id "
                                 "AND a.id = ma.album_id "
                                 "AND m.id = mi.musica_id "
                                 "AND a.nome ~* ?";

const QString playlistSearchQuery = "SELECT DISTINCT p.nome, p.utilizador_nick "
                                    "FROM playlist p "
                                    "WHERE p.private = false "
                                    "AND p.nome ~* ?";

const QString interpreterSearchQuery = "SELECT DISTINCT i.nome, i.datainicio "
                                       "FROM interprete i "
                                       "WHERE i.nome ~* ?";

const QString concertSearchQuery = "SELECT DISTINCT c.nome, c.data "
                                   "FROM concerto c "
                                   "WHERE c.nome ~* ?";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->homePage);
    stackedWidgetHistory.append(ui->homePage);
    stackedWidgetHistoryIndex = 0;
    ui->returnButton->setDisabled(true);
    ui->fowardButton->setDisabled(true);

    if (QSqlDatabase::drivers().isEmpty())
        QMessageBox::information(this, tr("No database drivers found"),
                                 tr("This program requires at least one Qt database driver. "
                                    "Please check the documentation how to build the "
                                    "Qt SQL plugins."));
    QString DBdriver;
    QString DBname;
    QString DBhost;
    int DBport;
    QString DBuser;
    QString DBpasswd;

    // Load setting from file
    // if not available use default
    DBdriver  = "QPSQL";
    DBname    = "postgres";
    DBhost    = "localhost";
    DBport    = 5432;
    DBuser    = "postgres";
    DBpasswd  = "postgres";

    QFile file("DatabaseSettings.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {

        QTextStream in(&file);
        QStringList settings;

        while (!in.atEnd()) {
            settings.append(in.readLine());
        }
        if(settings.size() == 5){

            DBdriver  = settings.at(0);
            DBname    = settings.at(1);
            DBhost    = settings.at(2);
            DBport    = settings.at(3).toInt();
            DBuser    = settings.at(4);
            DBpasswd  = settings.at(5);
        }
        file.close();
    }


    //Start Database Connection
    QSqlError err;
    QSqlDatabase db = QSqlDatabase::addDatabase(DBdriver);
    db.setDatabaseName(DBname);
    db.setHostName(DBhost);
    db.setPort(DBport);

    if (!db.open(DBuser, DBpasswd)) {
        err = db.lastError();
        QSqlDatabase::removeDatabase(db.connectionName());
        if (err.type() != QSqlError::NoError)
            QMessageBox::warning(this, tr("Unable to open database"), tr("An error occurred while "
                                                                         "opening the connection: ") + err.text());
    }

    currentUser.clear();
    currentPK.clear();
    currentFileMusicID = -1;
    ui->musicFiles->clear();
    this->setHidden(true);

    configureAllSqlTableView();


    StartWindow *sw = new StartWindow(this);
    connect(sw,SIGNAL(acceptedUser(QString)),this,SLOT(loggedIn(QString)));
    connect(sw, SIGNAL(rejected()),this,SLOT(close()));
    //Read Remember File
    sw->open();

}

MainWindow::~MainWindow()
{
    delete ui;
}

// LogInOut
void MainWindow::loggedIn(QString username){


    // hide search widgets
    ui->interpGroupBox->hide();
    ui->musicsGroupBox->hide();
    ui->albumsGroupBox->hide();
    ui->concertsGroupBox->hide();
    ui->playlistsGroupBox->hide();

    this->setHidden(false);

    this->findChild<QDialog*>("StartWindow")->deleteLater();

    currentUser = username;


    // ///////////////////////////////////////////////////////////////////
    // Write Log Register
    //
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    if(query.prepare("INSERT INTO registoacesso (data, tipoacesso, utilizador_nick) VALUES ((select current_timestamp), :tipoacesso, :utilizador_nick)")){
        query.bindValue(":tipoacesso", "LogIn");
        query.bindValue(":utilizador_nick", username);

        if(!query.exec()){
            qDebug() << "ERROR: " << query.lastError();
        }
        qDebug() << "Inserted " << query.numRowsAffected() << " into registoacesso";
    }else
        qDebug() << "ERROR: " << query.lastError();

    refreshUserPanel();
    refreshNotifications();

    query.finish();
    query.clear();

    this->show();

}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!currentUser.isEmpty()){
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, tr("Exit Music Database"),
                                                                    tr("Are you sure?\n"));
        if (resBtn != QMessageBox::Yes) {
            event->ignore();
            return;
        } else {
            {
                QSqlDatabase db = QSqlDatabase::database();
                QSqlQuery query(db);

                if(query.prepare("INSERT INTO registoacesso (data, tipoacesso, utilizador_nick) VALUES ((select current_timestamp), :tipoacesso, :utilizador_nick)")){
                    query.bindValue(":tipoacesso", "LogOff");
                    query.bindValue(":utilizador_nick", currentUser);

                    if(!query.exec()){
                        qDebug() << "ERROR: " << query.lastError();

                    }
                    qDebug() << "Inserted " << query.numRowsAffected() << " into registoacesso";
                }else
                    qDebug() << "ERROR: " << query.lastError();

                query.finish();
                query.clear();
                db.close();
            }
            closeAllViews();
            QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
        }
    }

    event->accept();
}

// refresh
void MainWindow::refreshUserPanel(){

    ui->UserInfoBox->setTitle(currentUser);

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    query.prepare("SELECT primeironome, ultimonome, email, pais, bio, imagem "
                  "FROM utilizador "
                  "WHERE nick=:username");
    query.bindValue(":username",currentUser);
    query.exec();
    query.first();
    ui->Name->setText(query.value(0).toString() + " " + query.value(1).toString());
    ui->email->setText(query.value(2).toString());
    ui->country->setText(query.value(3).toString());
    ui->bio->setText(query.value(4).toString());
    if(!query.value(5).isNull()){
        QByteArray outByteArray = query.value(5).toByteArray();
        QPixmap outPixmap = QPixmap();
        outPixmap.loadFromData(outByteArray);

        ui->UserPic->setPixmap(outPixmap);
    }
    query.clear();

    query.prepare("SELECT tipoacesso \"Access\", to_char(data,'hh:mm DD-MM-YY') \"Date\" "
                  "FROM registoacesso "
                  "WHERE utilizador_nick=:username "
                  "ORDER BY data DESC");
    query.bindValue(":username",currentUser);
    query.exec();
    qobject_cast<QSqlQueryModel *>(ui->regTableView->model())->setQuery(query);

}
void MainWindow::refreshNotifications(){
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery *query = new QSqlQuery(db);

    query->prepare("SELECT nome \"Name\", data \"Date\", descricao \"Description\" "
                   "FROM notificacoes "
                   "WHERE lida = FALSE "
                   "AND utilizador_nick=:username "
                   "ORDER BY data DESC");
    query->bindValue(":username",currentUser);
    query->exec();
    qobject_cast<QSqlQueryModel *>(ui->unreadNotifTableView->model())->setQuery(*query);

    query = new QSqlQuery(db);

    query->prepare("SELECT nome \"Name\", data \"Date\", descricao \"Description\" "
                   "FROM notificacoes "
                   "WHERE lida = TRUE "
                   "AND utilizador_nick=:username "
                   "ORDER BY data DESC");
    query->bindValue(":username",currentUser);
    query->exec();
    qobject_cast<QSqlQueryModel *>(ui->readNotifTableView->model())->setQuery(*query);
}
void MainWindow::refreshFiles(){

    currentFileMusicID = -1;
    ui->musicFiles->clear();
    ui->pathLineEdit->clear();
    ui->comment->clear();
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery *query = new QSqlQuery(db);

    query->prepare("SELECT m.nome \"Name\", f.comentario \"Comment\", to_char(f.dataupload,'DD-MM-YY') \"Date\" "
                   "FROM musica m, ficheiro f "
                   "WHERE f.musica_id = m.id "
                   "AND f.utilizador_nick=:username");
    query->bindValue(":username",currentUser);
    query->exec();
    qobject_cast<QSqlQueryModel *>(ui->yourFilesTableView->model())->setQuery(*query);

    query = new QSqlQuery(db);

    query->prepare("SELECT m.nome \"Name\", f.comentario \"Comment\", to_char(f.dataupload,'DD-MM-YY') \"Date\" "
                   "FROM musica m, ficheiro f, utilizador_ficheiro uf "
                   "WHERE f.musica_id = m.id "
                   "AND f.utilizador_nick = uf.ficheiro_utilizador_nick "
                   "AND f.musica_id = uf.ficheiro_musica_id "
                   "AND f.utilizador_nick=:username");
    query->bindValue(":username",currentUser);
    query->exec();
    qobject_cast<QSqlQueryModel *>(ui->sharedFilesTableView->model())->setQuery(*query);

}
void MainWindow::refreshPlaylists(){
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery *query = new QSqlQuery(db);

    query->prepare("SELECT p.nome \"Name\" "
                   "FROM playlist p "
                   "WHERE p.private = false "
                   "AND p.utilizador_nick = :username ");
    query->bindValue(":username",currentUser);
    query->exec();
    qobject_cast<QSqlQueryModel *>(ui->publicPlayListTableView->model())->setQuery(*query);

    query = new QSqlQuery(db);

    query->prepare("SELECT p.nome \"Name\" "
                   "FROM playlist p "
                   "WHERE p.private = true "
                   "AND p.utilizador_nick = :username ");
    query->bindValue(":username",currentUser);
    query->exec();
    qobject_cast<QSqlQueryModel *>(ui->privatePlayListTableView->model())->setQuery(*query);

    query = new QSqlQuery(db);

    query->prepare("SELECT p.playlist_nome \"Name\" "
                   "FROM utilizador_playlist p "
                   "WHERE p.utilizador_nick = :username ");
    query->bindValue(":username",currentUser);
    query->exec();
    qobject_cast<QSqlQueryModel *>(ui->followingTableView->model())->setQuery(*query);
}

// set Page
void MainWindow::setCurrentPage(QWidget* page)
{
    ui->returnButton->setDisabled(false);
    ui->fowardButton->setDisabled(true);
    stackedWidgetHistory = stackedWidgetHistory.mid(0,stackedWidgetHistoryIndex+1);
    stackedWidgetHistoryIndex++;
    stackedWidgetHistory.append(page);
    ui->stackedWidget->setCurrentWidget(page);

}

//Main Pages
void MainWindow::on_homeButton_clicked()
{
    refreshNotifications();
    if(ui->stackedWidget->currentWidget() != (ui->homePage))
        setCurrentPage(ui->homePage);

}
void MainWindow::on_filesButton_clicked()
{
    refreshFiles();
    if(ui->stackedWidget->currentWidget() != (ui->uploadsPage))
        setCurrentPage(ui->uploadsPage);
}
void MainWindow::on_searchButton_clicked()
{
    QStringList keywords = ui->searchBar->text().split(" ", QString::SkipEmptyParts);

    if(!keywords.isEmpty()){
        int totalFound = 0;

        totalFound += searchMusic(keywords);
        totalFound += searchAlbum(keywords);
        totalFound += searchPlaylist(keywords);
        totalFound += searchConcert(keywords);
        totalFound += searchInterpreter(keywords);

        if(totalFound > 0)
            setCurrentPage(ui->searchPage);

        qDebug() << "Search Found " << totalFound << " results for: " << keywords;
    }
}
void MainWindow::on_playlistsButton_clicked()
{
    refreshPlaylists();
    if(ui->stackedWidget->currentWidget() != ui->playlistsUserPage)
        setCurrentPage(ui->playlistsUserPage);
}
void MainWindow::on_searchBar_returnPressed()
{
    on_searchButton_clicked();
}
void MainWindow::on_returnButton_clicked()
{
    stackedWidgetHistoryIndex--;
    ui->fowardButton->setDisabled(false);
    qDebug() << "FowardEnabled";
    qDebug() << "History: " << stackedWidgetHistoryIndex+1 << " of " << stackedWidgetHistory.size();

    if(stackedWidgetHistoryIndex == 0){
        ui->returnButton->setDisabled(true);
        qDebug() << "ReturnDisabled";
    }
    ui->stackedWidget->setCurrentWidget(stackedWidgetHistory.at(stackedWidgetHistoryIndex));

}
void MainWindow::on_fowardButton_clicked()
{
    stackedWidgetHistoryIndex++;
    ui->returnButton->setDisabled(false);
    qDebug() << "ReturnEnabled";
    qDebug() << "History: " << stackedWidgetHistoryIndex+1 << " of " << stackedWidgetHistory.size();
    if(stackedWidgetHistoryIndex+1 == stackedWidgetHistory.size()){
        ui->fowardButton->setDisabled(true);
        qDebug() << "FowardDisabled";
    }
    ui->stackedWidget->setCurrentWidget(stackedWidgetHistory.at(stackedWidgetHistoryIndex));
}

//PlayLists
void MainWindow::on_newPrivatePlayList_clicked()
{
    bool ok;
    QString playlistName = QInputDialog::getText(this,"New Playlist!","Playlist Name:",QLineEdit::Normal,"",&ok,Qt::SplashScreen);

    if(ok && !playlistName.isEmpty()){
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);
        query.prepare(QString("SELECT count(*) FROM playlist WHERE nome = '%1' AND utilizador_nick = '%2'").arg(playlistName).arg(currentUser));
        query.exec();
        query.first();
        if(query.value(0).toInt() != 0){
            QMessageBox::information(this, tr("Already Taken"),
                                     tr("You already have a playlist with that name!\nPlease choose another!"));
            query.finish();
            query.clear();
            return;
        }
    }
    else if(ok){
        QMessageBox::information(this,"Name Required","You must provide a name to the playlist!");
        return;
    }
    else
        return;

    PlayListEditor *editor = new PlayListEditor(this);
    editor->setName(playlistName);
    editor->setUsername(currentUser);
    connect(editor,SIGNAL(searchMusic(QStringList)),this,SLOT(refreshPlaylistEditorBrowser(QStringList)));
    connect(editor,SIGNAL(failure()),this,SLOT(databaseFailure()));
    connect(editor,SIGNAL(accepted()),this,SLOT(playlistCreated()));
    connect(editor,SIGNAL(rejected()),this,SLOT(cancelPlaylist()));
    editor->configure();
    editor->open();
}
void MainWindow::on_newPublicPlayList_clicked()
{
    on_newPrivatePlayList_clicked();
}
void MainWindow::on_editPrivatePlaylist_clicked()
{

}
void MainWindow::on_deletePrivatePlayList_clicked()
{
    QString playlistName = ui->privatePlayListTableView->currentIndex().data().toString();

    QMessageBox::StandardButton resBtn = QMessageBox::question(this,"Delete Playlist",QString("Playlist \"%1\" will be deleted!\nAre you sure?").arg(playlistName));
    if (resBtn != QMessageBox::Yes) {
        return;
    }
    QSqlQuery query(QSqlDatabase::database());


    query.prepare("DELETE FROM playlist "
                  "WHERE nome = ? "
                  "AND utilizador_nick = ?");
    query.addBindValue(playlistName);
    query.addBindValue(currentUser);
    if(query.exec()){
        QMessageBox::information(this,"Deleted",QString("The playlist \"%1\" was deleted").arg(playlistName));
        refreshPlaylists();
        return;
    }
    QMessageBox::information(this,"Error",QString("Failed to delete playlist \"%1\"").arg(playlistName));

}
void MainWindow::on_deletePublicPlayList_clicked()
{
    QString playlistName = ui->publicPlayListTableView->currentIndex().data().toString();

    QMessageBox::StandardButton resBtn = QMessageBox::question(this,"Delete Playlist",QString("Playlist \"%1\" will be deleted!\nAre you sure?").arg(playlistName));
    if (resBtn != QMessageBox::Yes) {
        return;
    }
    QSqlQuery query(QSqlDatabase::database());


    query.prepare("DELETE FROM playlist "
                  "WHERE nome = ? "
                  "AND utilizador_nick = ?");
    query.addBindValue(playlistName);
    query.addBindValue(currentUser);
    if(query.exec()){
        QMessageBox::information(this,"Deleted",QString("The playlist \"%1\" was deleted").arg(playlistName));
        refreshPlaylists();
        return;
    }
    QMessageBox::information(this,"Error",QString("Failed to delete playlist \"%1\"").arg(playlistName));
}

//Files
void MainWindow::on_chooseMusicButton_clicked()
{
    BrowseMusicDialog *dialog = new BrowseMusicDialog(this);
    connect(dialog,SIGNAL(searchMusic(QStringList)),this,SLOT(refreshMusicBrowser(QStringList)));
    connect(dialog,SIGNAL(accepted()),this,SLOT(musicSelected()));
    connect(dialog,SIGNAL(rejected()),this,SLOT(cancelMusicSelection()));
    dialog->open();
    dialog->showAllMusics();
}
void MainWindow::on_browseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose Music File"),
                                                    "/Ambiente de trabalho",
                                                    tr("Musics (*.mp3 *.mp4 *.wav)"));

    ui->pathLineEdit->setText(fileName);
}
void MainWindow::on_addFileButton_clicked()
{
    if(currentFileMusicID !=-1){


        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);

        QFile file(ui->pathLineEdit->text());
        if (!file.open(QIODevice::ReadOnly)) return;
        QByteArray blob = file.readAll();

        query.prepare("INSERT INTO ficheiro(dataupload, ficheiro, comentario, utilizador_nick, musica_id) "
                      "VALUES((select current_timestamp), ?, ?, ?, ?)");
        query.addBindValue(blob);
        query.addBindValue(ui->comment->toPlainText());
        query.addBindValue(currentUser);
        query.addBindValue(currentFileMusicID);
        query.exec();
        query.finish();
        refreshFiles();
    }
}

//SearchDetails
void MainWindow::on_musicTableView_clicked(const QModelIndex &index)
{
    ui->musicTableView->setColumnHidden(0, false);
    int musicID = ui->musicTableView->model()->itemData(index.siblingAtColumn(0)).value(0,-1).toInt();
    ui->musicTableView->setColumnHidden(0, true);
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    // Get Music Data
    query.prepare("SELECT m.nome, a.nome, i.nome, c.interprete_nome, g.genero_tipo, m.data, m.duracao, m.letra "
                  "FROM musica m, musica_album ma, album a, musica_interprete mi, interprete i, musica_genero g, musica_musico mc, musico c "
                  "WHERE m.id = ma.musica_id "
                  "AND a.id = ma.album_id "
                  "AND m.id = mi.musica_id "
                  "AND i.nome = mi.interprete_nome "
                  "AND i.datainicio = mi.interprete_datainicio "
                  "AND m.id = g.musica_id "
                  "AND m.id = mc.musica_id "
                  "AND c.interprete_nome = mc.musico_interprete_nome "
                  "AND c.interprete_datainicio = mc.musico_interprete_datainicio "
                  "AND m.id = :musicID");
    query.bindValue(":musicID", musicID);
    query.exec();
    query.first();

    ui->NameImageMusic->setText(query.value(0).toString());
    ui->albumMusicPage->setText(query.value(1).toString());
    ui->interpMusicPage->setText(query.value(2).toString());
    ui->authorMusicPage->setText(query.value(3).toString());
    ui->genreMusicPage->setText(query.value(4).toString());
    ui->yearMusicPage->setText(QString::number(query.value(5).toDate().year()));
    ui->durationMusicPage->setText(query.value(6).toDateTime().toString("h:mm:ss"));
    ui->lyricsMusicPage->setText(query.value(7).toString());

    query.prepare("SELECT u.nick, r.critica_pontuacao, r.critica_justificacao "
                  "FROM utilizador u, criticamusica r "
                  "WHERE u.nick = r.critica_utilizador_nick "
                  "AND musica_id = :musicID");
    query.bindValue(":musicID", musicID);
    query.exec();

    QSqlQueryModel* model = qobject_cast<QSqlQueryModel *>(ui->reviewsMusicTableView->model());
    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, tr("User"));
    model->setHeaderData(1, Qt::Horizontal, tr("Score"));
    model->setHeaderData(2, Qt::Horizontal, tr("Comment"));

    currentPK = QVariant(musicID);

    setCurrentPage(ui->musicPage);

}
void MainWindow::on_albumsTableView_clicked(const QModelIndex &index)
{
    ui->musicTableView->setColumnHidden(0, false);
    int albumID = ui->musicTableView->model()->itemData(index.siblingAtColumn(0)).value(0,-1).toInt();
    ui->musicTableView->setColumnHidden(0, true);
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    // Get Music Data
    query.prepare("SELECT a.nome, mi.interprete_nome, g.genero_tipo, to_char(a.datalancamento,'YYYY'), a.editora_nome, a.detalhes "
                  "FROM musica m, musica_album ma, album a, musica_interprete mi, musica_genero g "
                  "WHERE m.id = ma.musica_id "
                  "AND a.id = ma.album_id "
                  "AND m.id = mi.musica_id "
                  "AND m.id = g.musica_id "
                  "AND a.id = ?");
    query.addBindValue(albumID);
    query.exec();
    query.first();

    ui->NameImageAlbum->setText(query.value(0).toString());
    ui->interpAlbumPage->setText(query.value(1).toString());
    ui->genreAlbumPage->setText(query.value(2).toString());
    ui->yearAlbumPage->setText(query.value(3).toString());
    ui->editorAlbumPage->setText(query.value(4).toString());
    ui->detailsAlbumPage->setText(query.value(5).toString());

    query.prepare("SELECT m.nome, to_char(m.duracao,'hh:mm:ss') "
                  "FROM musica m, musica_album ma, album a "
                  "WHERE m.id = ma.musica_id "
                  "AND a.id = ma.album_id "
                  "AND a.id = ?");
    query.addBindValue(albumID);
    query.exec();

    QSqlQueryModel* model = qobject_cast<QSqlQueryModel *>(ui->musicsAlbumTableView->model());

    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, tr("Name"));
    model->setHeaderData(0, Qt::Horizontal, tr("Duration"));

    QSqlQuery query2(db);

    query2.prepare("SELECT u.nick, r.critica_pontuacao, r.critica_justificacao "
                  "FROM utilizador u, criticaalbum r "
                  "WHERE u.nick = r.critica_utilizador_nick "
                  "AND album_id = ?");
    query2.addBindValue(albumID);
    query2.exec();

    QSqlQueryModel* model2 = qobject_cast<QSqlQueryModel *>(ui->reviewsAlbumTableView->model());

    model2->setQuery(query2);
    model2->setHeaderData(0, Qt::Horizontal, tr("User"));
    model2->setHeaderData(1, Qt::Horizontal, tr("Score"));
    model2->setHeaderData(2, Qt::Horizontal, tr("Comment"));


    currentPK = QVariant(albumID);

    setCurrentPage(ui->albumPage);
}

// SubPages buttons
void MainWindow::on_reviewAlbumButton_clicked()
{
    ReviewDialog dialog(this);

    dialog.setMusicAuthor(ui->NameImageMusic->text(),ui->authorMusicPage->text());

    if (dialog.exec() != QDialog::Accepted){

        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);

        int score = dialog.getScore();
        QString review = dialog.getReview();


        query.prepare("INSERT INTO criticaalbum(album_id, critica_data, critica_pontuacao, critica_justificacao, critica_utilizador_nick) "
                      "VALUES(:id, (select current_timestamp), :pontuacao, :just, :nick);");
        query.bindValue(":id", currentPK.toInt());
        query.bindValue(":pontuacao", score);
        query.bindValue(":just", review);
        query.bindValue(":nick", currentUser);
        query.exec();


        query.finish();

        refreshPlaylists();
        return;
    }
}
void MainWindow::on_reviewMusicButton_clicked()
{
    ReviewDialog dialog(this);
    dialog.setMusicAuthor(ui->NameImageMusic->text(),ui->authorMusicPage->text());

    if (dialog.exec() != QDialog::Accepted){

        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);

        int score = dialog.getScore();
        QString review = dialog.getReview();

        query.prepare("INSERT INTO criticamusica(musica_id, critica_data, critica_pontuacao, critica_justificacao, critica_utilizador_nick) "
                      "VALUES(:id, (select current_timestamp), :pontuacao, :just, :nick);");
        query.bindValue(":id", currentPK.toInt());
        query.bindValue(":pontuacao", score);
        query.bindValue(":just", review);
        query.bindValue(":nick", currentUser);
        query.exec();


        query.finish();

        return;
    }
}

// Search
int MainWindow::searchMusic(QStringList keywords){

    QSqlQueryModel* model = qobject_cast<QSqlQueryModel *>(ui->musicTableView->model());

    QSqlQuery *query = buildSearchQuery(musicSearchQuery, keywords);
    if(query->exec()){
        model->setQuery(*query);
        if(model->rowCount() > 0){
            model->setHeaderData(0, Qt::Horizontal, tr("ID"));
            model->setHeaderData(1, Qt::Horizontal, tr("Name"));
            model->setHeaderData(2, Qt::Horizontal, tr("Album"));
            model->setHeaderData(3, Qt::Horizontal, tr("Interpreter"));
            ui->musicTableView->setColumnHidden(0, true);
            ui->musicsGroupBox->setVisible(true);
            qDebug() << "Search Found " << model->rowCount() << " musics for : " << keywords;
        }
        else ui->musicsGroupBox->setVisible(false);

        //        qDebug() << "QUERY: " << query->lastQuery();
    }else qDebug() << "ERROR exec: " << query->lastError();

    return model->rowCount();
}
int MainWindow::searchAlbum(QStringList keywords){

    QSqlQueryModel* model = qobject_cast<QSqlQueryModel *>(ui->albumsTableView->model());

    QSqlQuery *query = buildSearchQuery(albumSearchQuery, keywords);
    if(query->exec()){
        model->setQuery(*query);
        if(model->rowCount() > 0){
            model->setHeaderData(0, Qt::Horizontal, tr("ID"));
            model->setHeaderData(1, Qt::Horizontal, tr("Name"));
            model->setHeaderData(2, Qt::Horizontal, tr("Interpreter"));
            model->setHeaderData(3, Qt::Horizontal, tr("Year"));
            ui->albumsTableView->setColumnHidden(0, true);
            ui->albumsGroupBox->setVisible(true);
            qDebug() << "Search Found " << model->rowCount() << " albums for : " << keywords;
        }
        else ui->albumsGroupBox->setVisible(false);

        //qDebug() << "QUERY: " << query->lastQuery();
    }else qDebug() << "ERROR exec: " << query->lastError();

    return model->rowCount();
}
int MainWindow::searchInterpreter(QStringList keywords){
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel *>(ui->interpTableView->model());

    QSqlQuery *query = buildSearchQuery(interpreterSearchQuery, keywords);
    if(query->exec()){
        model->setQuery(*query);
        if(model->rowCount() > 0){
            model->setHeaderData(0, Qt::Horizontal, tr("Name"));
            model->setHeaderData(1, Qt::Horizontal, tr("Date"));
            ui->interpTableView->setColumnHidden(1, true);
            ui->interpGroupBox->setVisible(true);
            qDebug() << "Search Found " << model->rowCount() << " interpreters for : " << keywords;
        }
        else ui->interpGroupBox->setVisible(false);

        //        qDebug() << "QUERY: " << query->lastQuery();
    }else qDebug() << "ERROR exec: " << query->lastError();

    return model->rowCount();
}
int MainWindow::searchPlaylist(QStringList keywords){
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel *>(ui->playlistsTableView->model());

    QSqlQuery *query = buildSearchQuery(playlistSearchQuery, keywords);
    if(query->exec()){
        model->setQuery(*query);
        if(model->rowCount() > 0){
            model->setHeaderData(0, Qt::Horizontal, tr("Name"));
            model->setHeaderData(1, Qt::Horizontal, tr("User"));
            ui->playlistsGroupBox->setVisible(true);
            qDebug() << "Search Found " << model->rowCount() << " playlists for : " << keywords;
        }
        else ui->playlistsGroupBox->setVisible(false);

        //qDebug() << "QUERY: " << query->lastQuery();

    }else qDebug() << "ERROR exec: " << query->lastError();

    return model->rowCount();
}
int MainWindow::searchConcert(QStringList keywords){
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel *>(ui->concertsTableView->model());

    QSqlQuery *query = buildSearchQuery(concertSearchQuery, keywords);
    if(query->exec()){
        model->setQuery(*query);
        if(model->rowCount() > 0){
            model->setHeaderData(0, Qt::Horizontal, tr("Name"));
            model->setHeaderData(1, Qt::Horizontal, tr("Date"));
            ui->concertsGroupBox->setVisible(true);
        }
        else ui->concertsGroupBox->setVisible(false);
        //        qDebug() << "QUERY: " << query->lastQuery();
        qDebug() << "Search Found " << model->rowCount() << " concert for : " << keywords;
    }else qDebug() << "ERROR exec: " << query->lastError();

    return model->rowCount();
}
QSqlQuery *MainWindow::buildSearchQuery(QString searchQuery, QStringList keywords)
{
    QSqlQuery *query =new QSqlQuery(QSqlDatabase::database());

    query->prepare(searchQuery);

    query->addBindValue(keywords.join(searchRule));

    return query;
}

// Tables
void MainWindow::closeAllViews(){

    //Search Views
    qobject_cast<QSqlQueryModel *>(ui->musicTableView->model())->query().finish();
    qobject_cast<QSqlQueryModel *>(ui->albumsTableView->model())->query().finish();
    qobject_cast<QSqlQueryModel *>(ui->playlistsTableView->model())->query().finish();
    qobject_cast<QSqlQueryModel *>(ui->concertsTableView->model())->query().finish();
    qobject_cast<QSqlQueryModel *>(ui->interpTableView->model())->query().finish();

    //PlayLists Views
    qobject_cast<QSqlQueryModel *>(ui->publicPlayListTableView->model())->query().finish();
    qobject_cast<QSqlQueryModel *>(ui->privatePlayListTableView->model())->query().finish();
    qobject_cast<QSqlQueryModel *>(ui->followingTableView->model())->query().finish();

    //Home Views
    qobject_cast<QSqlQueryModel *>(ui->readNotifTableView->model())->query().finish();
    qobject_cast<QSqlQueryModel *>(ui->unreadNotifTableView->model())->query().finish();
    qobject_cast<QSqlQueryModel *>(ui->regTableView->model())->query().finish();

    //Files Views
    qobject_cast<QSqlQueryModel *>(ui->yourFilesTableView->model())->query().finish();
    qobject_cast<QSqlQueryModel *>(ui->sharedFilesTableView->model())->query().finish();

    //Music Views
    qobject_cast<QSqlQueryModel *>(ui->reviewsMusicTableView->model())->query().finish();

    //Interp Views
    qobject_cast<QSqlQueryModel *>(ui->memberGroupInterpTableView->model())->query().finish();
    qobject_cast<QSqlQueryModel *>(ui->albumsInterpTableView->model())->query().finish();
    qobject_cast<QSqlQueryModel *>(ui->compoitionsInterpTableView->model())->query().finish();

    //Album Views
    qobject_cast<QSqlQueryModel *>(ui->reviewsAlbumTableView->model())->query().finish();
    qobject_cast<QSqlQueryModel *>(ui->musicsAlbumTableView->model())->query().finish();

    //Playlist Views
    qobject_cast<QSqlQueryModel *>(ui->musicsPlayListTableView->model())->query().finish();

    //Editor Views
    qobject_cast<QSqlQueryModel *>(ui->albumsEditorTableView->model())->query().finish();

    //Concert Views
    qobject_cast<QSqlQueryModel *>(ui->interpConcertPageTableView->model())->query().finish();
}
void MainWindow::configureAllSqlTableView(){

    //Search Views
    configureTableView(ui->musicTableView);
    configureTableView(ui->albumsTableView);
    configureTableView(ui->playlistsTableView);
    configureTableView(ui->concertsTableView);
    configureTableView(ui->interpTableView);

    //PlayLists Views
    configureTableView(ui->publicPlayListTableView);
    configureTableView(ui->privatePlayListTableView);
    configureTableView(ui->followingTableView);

    //Home Views
    configureTableView(ui->readNotifTableView);
    configureTableView(ui->unreadNotifTableView);
    configureTableView(ui->regTableView);

    //Files Views
    configureTableView(ui->yourFilesTableView);
    configureTableView(ui->sharedFilesTableView);

    //Music Views
    configureTableView(ui->reviewsMusicTableView);

    //InterpTableView
    configureTableView(ui->memberGroupInterpTableView);
    configureTableView(ui->albumsInterpTableView);
    configureTableView(ui->compoitionsInterpTableView);

    //Album Views
    configureTableView(ui->reviewsAlbumTableView);
    configureTableView(ui->musicsAlbumTableView);

    //Playlist Views
    configureTableView(ui->musicsPlayListTableView);

    //Editor Views
    configureTableView(ui->albumsEditorTableView);

    //Concert Views
    configureTableView(ui->interpConcertPageTableView);

}
void MainWindow::configureTableView(QTableView* view){
    view->setModel(new QSqlQueryModel(view));
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setShowGrid(false);
    view->setGridStyle(Qt::NoPen);
    view->setSortingEnabled(false);
    view->verticalHeader()->setVisible(false);
    view->horizontalHeader()->setCascadingSectionResizes(true);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

//PlayListEditor
void MainWindow::refreshPlaylistEditorBrowser(QStringList keywords){

    QSqlQuery *query;// = new QSqlQuery(QSqlDatabase::database());
    PlayListEditor* editor = qobject_cast<PlayListEditor*>(this->findChild<QDialog*>("PlayListEditor"));

    if(editor == nullptr){
        return;
    }
    if(keywords.isEmpty()){

        query = new QSqlQuery(QSqlDatabase::database());
        QString allMusicQuery = musicSearchQuery;
        allMusicQuery.replace("AND m.nome ~* ?","");
        query->exec(allMusicQuery);
        editor->setAllMusics(query);
    }
    else{
        query = buildSearchQuery(musicSearchQuery,keywords);
        query->exec();
        editor->setAllMusics(query);
    }
}
void MainWindow::databaseFailure(){
    PlayListEditor* editor = qobject_cast<PlayListEditor*>(this->findChild<QDialog*>("PlayListEditor"));
    editor->deleteLater();
    QMessageBox::information(this,"Database error","Failure creating/editing Playlist!");
}
void MainWindow::playlistCreated(){
    PlayListEditor* editor = qobject_cast<PlayListEditor*>(this->findChild<QDialog*>("PlayListEditor"));
    editor->deleteLater();
    QMessageBox::information(this,"Success!","Playlist created succesfuly!");
    refreshPlaylists();
}
void MainWindow::cancelPlaylist(){
    PlayListEditor* editor = qobject_cast<PlayListEditor*>(this->findChild<QDialog*>("PlayListEditor"));
    editor->deleteLater();
}

//Music Browser
void MainWindow::refreshMusicBrowser(QStringList keywords){
    QSqlQuery *query;// = new QSqlQuery(QSqlDatabase::database());
    BrowseMusicDialog* browser = qobject_cast<BrowseMusicDialog*>(this->findChild<QDialog*>("BrowseMusicDialog"));

    if(browser == nullptr){
        return;
    }
    if(keywords.isEmpty()){

        query = new QSqlQuery(QSqlDatabase::database());
        QString allMusicQuery = musicSearchQuery;
        allMusicQuery.replace("AND m.nome ~* ?","");
        query->exec(allMusicQuery);
        browser->setMusics(query);
    }
    else{
        query = buildSearchQuery(musicSearchQuery,keywords);
        query->exec();
        browser->setMusics(query);
    }
}
void MainWindow::cancelMusicSelection(){
    BrowseMusicDialog* browser = qobject_cast<BrowseMusicDialog*>(this->findChild<QDialog*>("BrowseMusicDialog"));
    browser->deleteLater();
}
void MainWindow::musicSelected(){

    BrowseMusicDialog* browser = qobject_cast<BrowseMusicDialog*>(this->findChild<QDialog*>("BrowseMusicDialog"));

    ui->musicFiles->setText(browser->getSelectedName() + " - " + browser->getSelectedInterpreter());
    currentFileMusicID = browser->getSelectedID();
    browser->deleteLater();
}



