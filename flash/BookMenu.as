import gfx.io.GameDelegate;
import Shared.GlobalFunc;

class BookMenu extends MovieClip {
	static var PAGE_BREAK_TAG:String="[pagebreak]";

	static var NOTE_WIDTH:Number=400;
	static var NOTE_X_OFFSET:Number=20;
	static var NOTE_Y_OFFSET:Number=10;
	static var CACHED_PAGES:Number=4;

	static var BookMenuInstance:Object;


	var TTS_InitialSpeak:Boolean;

	/* Stage Elements */
	var ReferenceTextInstance:TextField;

	var BookPages:Array;
	var PageInfoA:Array;

	var bNote:Boolean;

	var iCurrentLine:Number;
	var iLeftPageNumber:Number;
	var iMaxPageHeight:Number;
	var iNextPageBreak:Number;
	var iPageSetIndex:Number;
	var iPaginationIndex:Number;

	var ReferenceText_mc:TextField;
	var ReferenceTextField:TextField;

	var RefTextFieldTextFormat:TextFormat;

	function BookMenu() {
		super();
		BookMenu.BookMenuInstance=this;
		BookPages=new Array  ;
		PageInfoA=new Array  ;
		iLeftPageNumber=0;
		iPageSetIndex=0;
		bNote=false;
		ReferenceText_mc=ReferenceTextInstance;
		RefTextFieldTextFormat=ReferenceText_mc.PageTextField.getTextFormat();
		TTS_InitialSpeak=false;
	}

	function onLoad():Void {
		ReferenceText_mc._visible=false;
		ReferenceTextField=ReferenceText_mc.PageTextField;
		ReferenceTextField.noTranslate=true;
		ReferenceTextField.setTextFormat(RefTextFieldTextFormat);
		iMaxPageHeight=ReferenceTextField._height;
		GameDelegate.addCallBack("SetBookText",this,"SetBookText");
		GameDelegate.addCallBack("TurnPage",this,"TurnPage");
		GameDelegate.addCallBack("PrepForClose",this,"PrepForClose");
	}

	function SetBookText(astrText:String,abNote:Boolean):Void {
		bNote=abNote;
		ReferenceTextField.verticalAutoSize="top";
		ReferenceTextField.SetText(astrText,true);
		if (abNote) {
			ReferenceTextField._width=BookMenu.NOTE_WIDTH;
		}
		PageInfoA.push({pageTop:0,pageHeight:iMaxPageHeight});
		iCurrentLine=0;
		iPaginationIndex=setInterval(this,"CalculatePagination",30);
		iNextPageBreak=iMaxPageHeight;
		SetLeftPageNumber(0);
	}

	function CreateDisplayPage(PageTop:Number,PageBottom:Number,aPageNum:Number):Void {
		var Page_mc:MovieClip=ReferenceText_mc.duplicateMovieClip("Page",getNextHighestDepth());
		var PageTextField_tf:TextField=Page_mc.PageTextField;
		PageTextField_tf.noTranslate=true;
		PageTextField_tf.SetText(ReferenceTextField.htmlText,true);
		var iLineOffsetTop:Number=ReferenceTextField.getLineOffset(ReferenceTextField.getLineIndexAtPoint(0,PageTop));
		var iLineOffsetBottom:Number=ReferenceTextField.getLineOffset(ReferenceTextField.getLineIndexAtPoint(0,PageBottom));
		PageTextField_tf.replaceText(0,iLineOffsetTop,"");
		PageTextField_tf.replaceText(iLineOffsetBottom-iLineOffsetTop,ReferenceTextField.length,"");
		PageTextField_tf.autoSize="left";
		if (bNote) {
			PageTextField_tf._width=BookMenu.NOTE_WIDTH;
			Page_mc._x=Stage.visibleRect.x+BookMenu.NOTE_X_OFFSET;
			Page_mc._y=Stage.visibleRect.y+BookMenu.NOTE_Y_OFFSET;
		} else {
			Page_mc._x=ReferenceText_mc._x;
			Page_mc._y=ReferenceText_mc._y;
		}
		Page_mc._visible=false;
		Page_mc.pageNum=aPageNum;
		BookPages.push(Page_mc);
	}

	function CalculatePagination() {
		var bLastPage:Boolean=false;

		for (iCurrentLine; iCurrentLine<=ReferenceTextField.numLines; iCurrentLine++) {
			var iLineOffsetCurrent:Number=ReferenceTextField.getLineOffset(iCurrentLine);
			var iLineOffsetNext:Number=ReferenceTextField.getLineOffset(iCurrentLine+1);
			var acharBoundaries:Object=ReferenceTextField.getCharBoundaries(iLineOffsetCurrent);
			var astrPageText:String=iLineOffsetNext==-1?ReferenceTextField.text.substring(iLineOffsetCurrent):ReferenceTextField.text.substring(iLineOffsetCurrent,iLineOffsetNext);
			astrPageText=GlobalFunc.StringTrim(astrPageText);
			if (acharBoundaries.bottom>iNextPageBreak||astrPageText==BookMenu.PAGE_BREAK_TAG||iCurrentLine>=ReferenceTextField.numLines) {
				var aPageDims:Object={pageTop:0,pageHeight:iMaxPageHeight};
				if (astrPageText==BookMenu.PAGE_BREAK_TAG) {
					aPageDims.pageTop=acharBoundaries.bottom+ReferenceTextField.getLineMetrics(iCurrentLine).leading;
					PageInfoA[PageInfoA.length-1].pageHeight=acharBoundaries.top-PageInfoA[PageInfoA.length-1].pageTop;
				} else {
					aPageDims.pageTop=acharBoundaries.top;
					PageInfoA[PageInfoA.length-1].pageHeight=aPageDims.pageTop-PageInfoA[PageInfoA.length-1].pageTop;
				}
				iNextPageBreak=aPageDims.pageTop+iMaxPageHeight;
				if (aPageDims.pageTop!=undefined||bNote) {
					PageInfoA.push(aPageDims);
				}
				bLastPage=true;
			}
		}
		if (iCurrentLine>=ReferenceTextField.numLines) {
			clearInterval(iPaginationIndex);
			iPaginationIndex=-1;
		}
		UpdatePages();
		if (! TTS_InitialSpeak) {
			TTS_InitialSpeak=true;
			TTS_SendBook();
			TTS_SendPages(true);
		}
	}

	function SetLeftPageNumber(aiPageNum:Number):Void {
		if (aiPageNum<PageInfoA.length) {
			iLeftPageNumber=aiPageNum;
		}
	}

	function ShowPageAtOffset(aiPageOffset:Number):Void {
		for (var i:Number=0; i<BookPages.length; i++) {
			if (BookPages[i].pageNum==iPageSetIndex+aiPageOffset) {
				BookPages[i]._visible=true;
			} else {
				BookPages[i]._visible=false;
			}
		}

	}
	function PrepForClose():Void {
		iPageSetIndex=iLeftPageNumber;
	}

	function TurnPage(aiDelta:Number):Boolean {
		var iNewPageNumber:Number=iLeftPageNumber+aiDelta;
		var bValidTurn:Boolean=iNewPageNumber>=0&&iNewPageNumber<PageInfoA.length;
		if (bNote) {
			bValidTurn=iNewPageNumber>=0&&iNewPageNumber<PageInfoA.length-1;
		}
		var iPagestoTurn:Number=Math.abs(aiDelta);
		if (bValidTurn) {
			var iMaxTurnablePages:Number=iPagestoTurn==1?1:4;
			SetLeftPageNumber(iNewPageNumber);
			if (iLeftPageNumber<iPageSetIndex) {
				iPageSetIndex=iPageSetIndex-iPagestoTurn;
			} else if (iLeftPageNumber>=iPageSetIndex+iMaxTurnablePages) {
				iPageSetIndex=iPageSetIndex+iPagestoTurn;
			}
			UpdatePages();
			TTS_SendPages(false);
		}
		return bValidTurn;
	}

	function UpdatePages():Void {
		for (var i:Number=0; i<BookMenu.CACHED_PAGES; i++) {
			var bCachedPage:Boolean=false;

			for (var j=0; j<BookPages.length&&! bCachedPage; j++) {
				if (BookPages[j].pageNum==iPageSetIndex+i) {
					bCachedPage=true;
				}

			}
			if (! bCachedPage&&PageInfoA.length>iPageSetIndex+i+1||iPaginationIndex==-1&&PageInfoA.length>iPageSetIndex+i) {
				CreateDisplayPage(PageInfoA[iPageSetIndex+i].pageTop,PageInfoA[iPageSetIndex+i].pageTop+PageInfoA[iPageSetIndex+i].pageHeight,iPageSetIndex+i);
			}
		}

		for (var i:Number=0; i<BookPages.length; i++) {
			if (BookPages[i].pageNum<iPageSetIndex||BookPages[i].pageNum>=iPageSetIndex+BookMenu.CACHED_PAGES) {
				BookPages.splice(i,1)[0].removeMovieClip();
			}
		}
	}
	function TTS_SendPages(isFirst:Boolean) {
		var pageText="";
		for (var i:Number=0; i<BookPages.length; i++) {
			if (this.BookPages[i]&&this.BookPages[i].pageNum==this.iLeftPageNumber&&this.BookPages[i].PageTextField&&this.BookPages[i].PageTextField.text) {
				pageText+=this.BookPages[i].PageTextField.text;
				break;
			}
		}
		if (! this.bNote) {
			for (var i:Number=0; i<BookPages.length; i++) {
				if (this.BookPages[i]&&this.BookPages[i].pageNum==this.iLeftPageNumber+1&&this.BookPages[i].PageTextField&&this.BookPages[i].PageTextField.text) {
					pageText+=this.BookPages[i].PageTextField.text;
					break;
				}
			}
		}
		if (pageText.length>0) {
			if(isFirst){
				_global.skse.plugins.FuzRoBork.BorkFunction(pageText,"BOOK_PAGES_FIRST");
			}
			else{
				_global.skse.plugins.FuzRoBork.BorkFunction(pageText,"BOOK_PAGES");
			}
		}
	}
	function TTS_SendBook() {
		var pageText="";
		for (var i:Number=0; i<BookPages.length; i++) {
			if (this.BookPages[i]&&this.BookPages[i].PageTextField&&this.BookPages[i].PageTextField.text) {
				pageText+=this.BookPages[i].PageTextField.text;
			}
		}
		if (pageText.length>0) {
			_global.skse.plugins.FuzRoBork.BorkFunction(pageText,"BOOK_BOOK");
		}
	}
}