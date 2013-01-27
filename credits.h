/**
 * @file credits.h
 * @author Robert Grandin
 * @date 26 January 2013
 * @brief Definition of Credits class.
 *
 * @section Class Description & Notes
 *
 * This class defines a dialog box which displays credits (e.g., attribution for
 * use of free icons) for the application.
 *
 *
 * All functions contained within this class are intended for use with the GNU
 * C++ compiler (g++).  Use with other compilers may produce unexpected results
 * and such use is at the users' own risk.
 *
 *
 * @section Revisions
 *
 * @date 27 January 2013
 *	- Creation date.
 *
 *
 *
 *
 * @section License
 *
 * Copyright (c) 2013, Robert Grandin
 * All rights reserved.
 *
 * Redistribution and use of this file is permitted provided that the following
 * conditions are met:
 * 	-# 	Redistributions must produce the above copyright notice, this list of
 * 		conditions, and the following disclaimer in the documentation and/or
 * 		other materials provided with the distribution.
 * 	-#	Neither the name of the organization nor the names of its contributors
 * 		may be used to endorse or promote products derived from this software
 * 		without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
 * EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING BUT NOT
 * LIMITING TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 *
 */



#ifndef CREDITS_H
#define CREDITS_H

#include <QDialog>

/** @brief Namespace used for user interface. */
namespace Ui {

/** @brief Dialog box to display application credits. */
class Credits;
}


/** @brief Dialog box to display application credits. */
class Credits : public QDialog
{
    Q_OBJECT
    
public:
    /**
     * @brief Credits constructor.
     * @param parent Parent widget.
     */
    explicit Credits(QWidget *parent = 0);

    /** @brief Destructor. */
    virtual ~Credits();
    
private:
    /** @brief Pointer to interface. */
    Ui::Credits *ui;
};

#endif // CREDITS_H
