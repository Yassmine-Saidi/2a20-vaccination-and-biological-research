#include "carnets.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

Carnets::Carnets() {}

Carnets::Carnets(int Id_Carnet, QString Cin, QString Nom, QString Prenom, int Age,
                 QString Sexe, QString Num, float Poids, QDate Date_Rdv,
                 QString Remarques, QString Statut_Vaccinal)
{
    this->Id_Carnet = Id_Carnet;
    this->Cin = Cin;
    this->Nom = Nom;
    this->Prenom = Prenom;
    this->Age = Age;
    this->Sexe = Sexe;
    this->Num = Num;
    this->Poids = Poids;
    this->Date_Rdv = Date_Rdv;
    this->Remarques = Remarques;
    this->Statut_Vaccinal = Statut_Vaccinal;
}

bool Carnets::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO CARNETS (CIN, NOM, PRENOM, AGE, SEXE, NUM, POIDS, DATE_RDV, REMARQUES, STATUT_VACCINAL) "
                  "VALUES (:cin, :nom, :prenom, :age, :sexe, :num, :poids, :date_rdv, :remarques, :statut)");

    query.bindValue(":cin", Cin);
    query.bindValue(":nom", Nom);
    query.bindValue(":prenom", Prenom);
    query.bindValue(":age", Age);
    query.bindValue(":sexe", Sexe);
    query.bindValue(":num", Num);
    query.bindValue(":poids", Poids);
    query.bindValue(":date_rdv", Date_Rdv);
    query.bindValue(":remarques", Remarques);
    query.bindValue(":statut", Statut_Vaccinal);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout du carnet :" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* Carnets::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT ID_CARNET, CIN, NOM, PRENOM, AGE, SEXE, NUM, POIDS, DATE_RDV, REMARQUES, STATUT_VACCINAL FROM CARNETS");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur lors de l'affichage des carnets :" << model->lastError().text();
        return nullptr;
    }

    // 🛠️ Ajustement des index pour correspondre à la requête SQL
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Âge"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Sexe"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Poids"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Date RDV"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Remarques"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("Statut Vaccinal"));

    return model;
}

bool Carnets::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM CARNETS WHERE Id_Carnet= :id");
    query.bindValue(":id", id);
    return query.exec();  // Retourne true si la suppression réussit
}
bool Carnets::modifier(int id, QString cin, QString nom, QString prenom, int age, QString sexe,
                       QString num, float poids, QDate date_rdv, QString remarques, QString statut_vaccinal)
{
    QSqlQuery query;
    query.prepare("UPDATE CARNETS SET CIN = :cin, NOM = :nom, PRENOM = :prenom, AGE = :age, "
                  "SEXE = :sexe, NUM = :num, POIDS = :poids, DATE_RDV = :date_rdv, "
                  "REMARQUES = :remarques, STATUT_VACCINAL = :statut WHERE Id_Carnet = :id");

    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":age", age);
    query.bindValue(":sexe", sexe);
    query.bindValue(":num", num);
    query.bindValue(":poids", poids);
    query.bindValue(":date_rdv", date_rdv);
    query.bindValue(":remarques", remarques);
    query.bindValue(":statut", statut_vaccinal);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la modification du carnet :" << query.lastError().text();
        return false;
    }
    return true;
}

bool Carnets::remplirChampsModification(int id, QString &cin, QString &nom, QString &prenom, int &age,
                                        QString &sexe, QString &num, float &poids, QDate &date_rdv,
                                        QString &remarques, QString &statut_vaccinal)
{
    QSqlQuery query;
    query.prepare("SELECT CIN, NOM, PRENOM, AGE, SEXE, NUM, POIDS, DATE_RDV, REMARQUES, STATUT_VACCINAL "
                  "FROM CARNETS WHERE Id_Carnet = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur lors du chargement du carnet :" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        cin = query.value(0).toString();
        nom = query.value(1).toString();
        prenom = query.value(2).toString();
        age = query.value(3).toInt();
        sexe = query.value(4).toString();
        num = query.value(5).toString();
        poids = query.value(6).toFloat();
        date_rdv = query.value(7).toDate();
        remarques = query.value(8).toString();
        statut_vaccinal = query.value(9).toString();
        return true;
    }
    return false;
}
