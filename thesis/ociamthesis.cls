%
% ociamthesis v2.2
% By Keith A. Gillow <gillow@maths.ox.ac.uk>
% Version 1.0 released 26/11/1997
%-------------------------- identification ---------------------
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{ociamthesis}[2010/11/22 v2.2 OCIAM thesis class]
%-------------------------- initial code -----------------------
\LoadClass[a4paper, 12pt]{report}
% As an alternative to the above could use next line for twosided output
%\LoadClass[a4paper,twoside,openright]{report}

\RequirePackage{graphicx} % needed for latest frontpage logo

\raggedbottom

%
% DECLARATIONS
%
% These macros are used to declare arguments needed for the
% construction of the title page and other preamble.

\def\degreedate#1{\gdef\@degreedate{#1}}
\def\degree#1{\gdef\@degree{#1}}
\def\college#1{\gdef\@college{#1}}
\def\albumnumber#1{\gdef\@albumnumber{#1}}
\def\promotor#1{\gdef\@promotor{#1}}
\def\logo{{\includegraphics[width=\textwidth]{pjatklogo}}}

%
% Define text area of page and margin offsets
% \setlength{\topmargin}{0.0in}
% \setlength{\oddsidemargin}{0.33in}
% \setlength{\evensidemargin}{-0.33in}
\setlength{\textheight}{22cm}
\setlength{\textwidth}{14cm}
\setlength\oddsidemargin{\dimexpr(\paperwidth-\textwidth)/2 - 1in\relax}
\setlength\evensidemargin{\oddsidemargin}

%
% Environments
%

% This macro define an environment for front matter that is always 
% single column even in a double-column document.

\newenvironment{alwayssingle}{%
       \@restonecolfalse
       \if@twocolumn\@restonecoltrue\onecolumn
       \else\if@openright\cleardoublepage\else\clearpage\fi
       \fi}%
       {\if@restonecol\twocolumn
       \else\newpage\thispagestyle{empty}\fi}

%define title page layout
\renewcommand{\maketitle}{
\begin{alwayssingle}
    \renewcommand{\footnotesize}{\small}
    \renewcommand{\footnoterule}{\relax}
    \thispagestyle{empty}
  \begin{center}
    {{\logo \par} \vspace*{10mm}}
    
    {{\Large {\bfseries {Wydział Informatyki}}}}
    \vspace*{10mm}

    {{\Large {\bfseries {Katedra Systemów Inteligentnych i Data Science}}}}
    \vspace*{10mm}

    {{\Large Data Science} \par}
    \vspace*{15mm}
    {{\Large {\bfseries {\@author}} \par}}
    {{Nr albumu {\@albumnumber}} \par}
    \vspace*{15mm}
    { \Huge {\bfseries {\@title}} \par}
    {\large \vspace*{30mm}
    {\it {\@degree} \par}
    \vspace*{2ex}
    {\it {\@promotor} \par}
    \vspace*{20mm}
    {\@degreedate}}
  \end{center}
  % \null\vfill
\end{alwayssingle}
}

%ABSTRACT
%
%The abstract environment puts a large, bold, centered "Abstract" label at
%the top of the page. The abstract itself appears in a quote environment,
%i.e. tabbed in at both sides, and on its own page.

\renewenvironment{abstract} {\begin{alwayssingle} \thispagestyle{empty}
  \begin{center}
  \vspace*{1.5cm}
  {\Large \bfseries  Streszczenie}
  \end{center}
  \vspace{0.5cm}
   }
{\end{alwayssingle}}

%ROMANPAGES
%
% The romanpages environment set the page numbering to lowercase roman one
% for the contents and figures lists. It also resets
% page-numbering for the remainder of the dissertation (arabic, starting at 1).

\newenvironment{romanpages}
{\cleardoublepage\setcounter{page}{1}\renewcommand{\thepage}{\roman{page}}}
{\cleardoublepage\renewcommand{\thepage}{\arabic{page}}\setcounter{page}{1}}
