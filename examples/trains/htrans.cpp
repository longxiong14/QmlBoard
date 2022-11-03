#include "htrans.h"

#include <QCoreApplication>
#include <QDir>
#include <QLocale>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickView>

#include "../../Common/hjsoncommon.h"

const static auto cEnglisthStr = QStringLiteral("English");
const static auto cChineseStr = QStringLiteral("简体中文");
HTranslator::HTranslator(QObject *parent) : QTranslator(parent) {}

void HTranslator::beforeUiReady(QQmlContext *ctx, const QString &folder) {
  m_ctx = ctx;
  ctx->setContextProperty("trans", this);
  if (folder.isEmpty()) {
    loadFolder(qApp->applicationDirPath() + "/Trans");
  } else {
    loadFolder(folder);
  }
  qApp->installTranslator(this);
}

void HTranslator::afterUiReady() {}

QString HTranslator::translate(const char *context, const char *sourceText,
                               const char *disambiguation, int n) const {
  Q_UNUSED(context)
  Q_UNUSED(disambiguation)
  Q_UNUSED(n)

  return trans(sourceText);
}

void HTranslator::loadFolder(const QString &folder) {
  qWarning() << "Trans loadFolder" << folder;
  QDir dir(folder);
  auto infos = dir.entryInfoList({"language_*.json"}, QDir::Files);
  QString lang;
  for (const auto &info : infos) {
    load(lang, info.absoluteFilePath());
  }

  initEnglish();
  auto langs = m_map.keys();
  if (langs.contains(cChineseStr)) {
    langs.removeAll(cChineseStr);
    langs.push_front(cChineseStr);
  }
  setLanguages(langs);
  if (m_map.contains(cChineseStr)) {
    setCurrentLang(cChineseStr);
  } else {
    setCurrentLang(cEnglisthStr);
  }
  emit folderLoaded(folder);
}

bool HTranslator::load(QString &lang, const QString &filePath) {
  lang.clear();
  QJsonObject rootObj;
  if (0 != HJsonCommon::readJsonObject(filePath, rootObj)) {
    return false;
  }
  lang = rootObj.value("lang").toString();
  const auto &trans = rootObj.value("trans").toArray();
  for (const auto &i : trans) {
    auto transObj = i.toObject();
    QString key = transObj.value("key").toString();
    QString value = transObj.value("value").toString();
    m_map[lang][key] = value;
  }

  emit langLoaded(lang);
  return true;
}

const QString &HTranslator::currentLang() const { return m_currentLang; }

const QStringList &HTranslator::languages() const { return m_languages; }

const QString &HTranslator::transString() const { return m_transString; }

void HTranslator::initEnglish() {
  if (!m_map.contains(cEnglisthStr)) {
    QHash<QString, QString> map;
    if (m_map.contains(cChineseStr)) {
      map = m_map.value(cChineseStr);
    } else {
      map = m_map.value(m_map.keys().first());
    }
    for (const auto &key : map.keys()) {
      m_map[cEnglisthStr][key] = key;
    }
  }
}

QString HTranslator::trans(const QString &source) const {
  return m_map.value(m_currentLang).value(source, source);
}

void HTranslator::setCurrentLang(const QString &currentLang) {
  if (m_currentLang == currentLang) return;

  m_currentLang = currentLang;
  emit currentLangChanged(m_currentLang);

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
  m_ctx->engine()->retranslate();
#else
  emit transStringChanged();
#endif
}

void HTranslator::setLanguages(const QStringList &languages) {
  if (m_languages == languages) return;

  m_languages = languages;
  emit languagesChanged(m_languages);
}
