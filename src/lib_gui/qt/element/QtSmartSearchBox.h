#ifndef QT_SMART_SEARCH_BOX_H
#define QT_SMART_SEARCH_BOX_H

#include <deque>
#include <memory>

#include <QLineEdit>
#include <QPushButton>

#include "data/search/SearchMatch.h"
#include "data/NodeTypeSet.h"
#include "qt/element/QtAutocompletionList.h"

class QtSearchElement
	: public QPushButton
{
	Q_OBJECT

signals:
	void wasChecked(QtSearchElement*);

public:
	QtSearchElement(const QString& text, QWidget* parent);

private slots:
	void onChecked(bool);
};


class QtSmartSearchBox
	: public QLineEdit
{
	Q_OBJECT

public slots:
	void search();
	void fullTextSearch();

public:
	QtSmartSearchBox(QWidget* parent);
	virtual ~QtSmartSearchBox();

	QCompleter* getCompleter() const;

	void setAutocompletionList(const std::vector<SearchMatch>& autocompletionList);
	void setMatches(const std::vector<SearchMatch>& matches);
	void setFocus();
	void findFulltext();

	void refreshStyle();

protected:
	virtual bool event(QEvent *event);
	virtual void resizeEvent(QResizeEvent* event);
	virtual void focusInEvent(QFocusEvent* event);

	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);

	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);

private slots:
	void onTextEdited(const QString& text);
	void onTextChanged(const QString& text);

	void onAutocompletionHighlighted(const SearchMatch& match);
	void onAutocompletionActivated(const SearchMatch& match);

	void onElementSelected(QtSearchElement* element);

private:
	void moveCursor(int offset);
	void moveCursorTo(int goal);

	void addMatch(const SearchMatch& match);
	void addMatchAndUpdate(const SearchMatch& match);
	void clearMatches();

	void setEditText(const QString& text);
	bool editTextToElement();
	SearchMatch editElement(QtSearchElement* element);

	void updateElements();
	void layoutElements();

	bool hasSelectedElements() const;
	std::string getSelectedString() const;

	void selectAllElementsWith(bool selected);
	void selectElementsTo(size_t idx, bool selected);
	void deleteSelectedElements();

	void updatePlaceholder();
	void clearLineEdit();

	void requestAutoCompletions();
	void hideAutoCompletions();

	std::deque<SearchMatch> getMatchesForInput(const std::string& text) const;

	NodeTypeSet getMatchAcceptedNodeTypes() const;
	bool lastMatchIsNoFilter() const;

	bool m_allowTextChange;
	QString m_oldText;

	std::deque<SearchMatch> m_matches;
	SearchMatch m_oldMatch;

	std::vector<QtSearchElement*> m_elements;

	size_t m_cursorIndex;

	SearchMatch m_highlightedMatch;

	bool m_shiftKeyDown;
	bool m_mousePressed;
	int m_mouseX;
	bool m_ignoreNextMousePress;

	QWidget* m_highlightRect;
	QtAutocompletionList* m_completer;

	int m_oldLayoutOffset = 0;
};

#endif // QT_SMART_SEARCH_BOX_H
