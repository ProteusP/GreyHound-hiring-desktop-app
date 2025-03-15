#include "mainpage.h"
#include "customwidgets.h"
#include "flowlayout.h"
#include "ui_mainpage.h"

MainPage::MainPage(QWidget *parent) : QWidget(parent), ui(new Ui::MainPage) {
  ui->setupUi(this);
  FlowLayout *flowLayout = new FlowLayout;
  flowLayout->addWidget(new VacancyCard("senior developer python", "ноунейм1",
                                        "топ вакансия мильон в сутки"));
  flowLayout->addWidget(new VacancyCard("senior developer c++", "ноунейм2",
                                        "топ вакансия мильон в сутки"));
  flowLayout->addWidget(new VacancyCard("senior developer go", "ноунейм3",
                                        "топ вакансия мильон в сутки"));
  flowLayout->addWidget(new VacancyCard(
      "senior developer assembler", "ноунейм4", "топ вакансия мильон в сутки"));
  flowLayout->addWidget(new VacancyCard("senior developer pascal", "ноунейм5",
                                        "топ вакансия мильон в сутки"));
  ui->groupBoxVacancies_3->setLayout(flowLayout);
}

MainPage::~MainPage() { delete ui; }

void MainPage::on_profilePB_3_clicked() { emit onProfilePressed(); }
