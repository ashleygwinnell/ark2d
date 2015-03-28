package com.adobe.flascc
{
  import flash.display.Bitmap;
  import flash.display.BitmapData;
  import flash.display.DisplayObjectContainer; 
  import flash.display.InteractiveObject; 
  import flash.display.MovieClip; 
  import flash.display.Sprite;
  import flash.display.Stage;
  import flash.display.Stage3D;
  import flash.display.StageAlign;
  import flash.display.StageScaleMode;
  import flash.display.StageDisplayState;
  import flash.display3D.Context3D;
  import flash.display3D.Context3DProfile;
  import flash.display3D.Context3DRenderMode;
  //import flash.display.*;
  import flash.display3D.*;
  import flash.display3D.textures.*;
  import flash.events.AsyncErrorEvent;
  import flash.events.Event;
  import flash.events.EventDispatcher;
  import flash.events.IOErrorEvent;
  import flash.events.KeyboardEvent;
  import flash.events.MouseEvent;
  import flash.events.FocusEvent;
  import flash.events.ProgressEvent;
  import flash.events.SampleDataEvent;
  import flash.events.SecurityErrorEvent;
  import flash.geom.Rectangle;
  import flash.geom.*;
  import flash.media.Sound;
  import flash.media.SoundChannel;
  import flash.media.SoundTransform;
  import flash.net.LocalConnection;
  import flash.net.URLLoader;
  import flash.net.URLLoaderDataFormat;
  import flash.net.URLRequest;
  import flash.text.TextField;
  import flash.text.TextFormat;
  import flash.trace.Trace;
  import flash.ui.Keyboard;
  
  //import flash.ui.GameInput;
  //import flash.ui.GameInputDevice; 
  //import flash.ui.GameInputControl;
  //import flash.events.GameInputEvent;
  
  //import flash.events.Event;
  import flash.system.Capabilities;

  import flash.utils.ByteArray;
  import flash.utils.Endian;
  import flash.utils.getTimer;
  import flash.utils.*;


  //import mx.controls.Alert;
  //import fl.controls.Button;

  import com.adobe.utils.*;
  import com.adobe.utils.macro.*;
  import com.adobe.flascc.CModule; 
  import com.adobe.flascc.vfs.*;
  import com.adobe.flascc.vfs.gamevfs;
  import com.adobe.flascc.vfs.InMemoryBackingStore;
  import com.adobe.flascc.vfs.LSOBackingStore;
  import com.adobe.flascc.vfs.ISpecialFile;
  import com.adobe.flascc.vfs.zip.*;
  import GLS3D.GLAPI;

  /*class ZipBackingStore extends InMemoryBackingStore {
    public function ZipBackingStore()
    {
      addDirectory("/root")
      addDirectory("/root/data")
    }

    public function addZip(data:ByteArray) {
      var zip = new ZipFile(data)
      for (var i = 0; i < zip.entries.length; i++) {
        var e = zip.entries[i]
        if (e.isDirectory()) {
          addDirectory("/root/data/"+e.name)
        } else {
          addFile("/root/data/"+e.name, zip.getInput(e))
        }
      }
    }
  }

  var zfs:ZipBackingStore = new ZipBackingStore();
  public function addVFSZip(x:*) {
    if(!zfs) {
      zfs = new ZipBackingStore();
    }
    zfs.addZip(x)
  }*/

   public class TestLog
    {
        //private const emptyVec:Vector.<int> = new Vector.<int>();
        public static function send(value:String):void
        {
            //trace("LOG: " + value);
            //var threadedLog2Ptr:int = CModule.getPublicSymbol("_Z11flascc_log2Ss");
            //CModule.callI(threadedLog2Ptr, )
            var emptyVec:Vector.<int> = new Vector.<int>();

            var threadedLogPtr:int = CModule.getPublicSymbol("_Z10flascc_logv");
            var threadedLogStrPtr:int = CModule.getPublicSymbol("flascc_logstr");
            

            var stringptr:int = CModule.mallocString(value);
            CModule.write32(threadedLogStrPtr, stringptr);
            CModule.callI(threadedLogPtr, emptyVec);

        }

        public static function sendObject(o:Object):void
        {
            var str:String = "";
            for (var val:* in o) {
                str += "   [" + typeof(o[val]) + "] " + val + " => " + o[val];
            }
            TestLog.send(str);
        }
    }



    //import flash.events.Event;
    //import flash.events.MouseEvent;
    //import flash.events.FocusEvent;
    //import flash.display.Sprite;
    //import flash.display.Stage;

     public class ModalCoverSprite extends Sprite
    {
        public function ModalCoverSprite()
        {
            super();
          
            addEventListener(Event.ADDED_TO_STAGE, onAdded); 

            TestLog.send("ModalCoverSprite.__constructor done.");
        }

        private function fitToBrowser():void 
        {
            //width = stage.stageWidth/2;
           // height = stage.stageHeight/2;
        }
 
        private function onBrowserResize(event:Event):void
        {
            fitToBrowser();
        }

        private function onAdded(event:Event):void 
        {
            TestLog.send("ModalCoverSprite.onAdded");
      
            var thisHeight:int = 150; // stage.stageHeight / 2; // 150; 
      
            graphics.beginFill( 0, 0.3 ); //alpha can be 0 as well.
            graphics.drawRect( 0, 0, stage.stageWidth / 2, thisHeight  );
       
            graphics.endFill( );  
      
            graphics.beginFill( 0, 0.5 ); //alpha can be 0 as well.
            graphics.drawRect( 0, 0, stage.stageWidth / 2,  2 );
            graphics.drawRect( 0, 45, stage.stageWidth / 2,  2 );
            graphics.drawRect( 0, 0, 2,  thisHeight ); 
            
            graphics.drawRect( 0, thisHeight, stage.stageWidth / 2,  2 );
            //graphics.drawRect( 0, -2 + ( stage.stageWidth / 2), 2,  stage.stageHeight / 2 );
            graphics.drawRect( stage.stageWidth / 2, 0, 2,  thisHeight ); 
            graphics.endFill( ); 
            
            width = stage.stageWidth/2;
            height = thisHeight;
              
            x = (stage.stageWidth / 2) - (width / 2);
            y = (stage.stageHeight / 2) - (height / 2);

            fitToBrowser();
            stage.addEventListener( Event.RESIZE, onBrowserResize );
            addEventListener(Event.REMOVED_FROM_STAGE, onRemoved);
        }

        protected function onRemoved(event:Event):void
        {
            TestLog.send("ModalCoverSprite.onRemoved");

            stage.removeEventListener( Event.RESIZE, onBrowserResize );
            removeEventListener(Event.REMOVED_FROM_STAGE, onRemoved);
        }
    } 

    public class Message
    {

        private static var s_stage : Stage = null;
        private static var overlay : Sprite = null;
        private static var titleText : TextField = null;
        private static var contentText : TextField = null;
        private static var okButton : TextField = null;
        private static var cancelButton : TextField = null;
     
        private static var s_isOpen:Boolean = false;
     
        private static var previousFocus:InteractiveObject = null;
        private static var previousText:String = new String("");
     
        public static function getText():String {
          if (contentText == null) {
            return previousText;
          } 
          return contentText.text; 
        }

        public static var m_callbackId:int = 0;

        public static function isOpen():Boolean {
          return s_isOpen;
        }

        protected static function onOkButtonClick(event:MouseEvent):void {
            TestLog.send("Message.onOkButtonClick");
            close();   
        }
        protected static function onOkButtonRemoved(event:Event):void {
            TestLog.send("Message.onOkButtonRemoved"); 
            okButton.removeEventListener(MouseEvent.CLICK, onOkButtonClick);
            okButton.removeEventListener(Event.REMOVED_FROM_STAGE, onOkButtonRemoved);
        }

        protected static function onCancelButtonClick(event:MouseEvent):void {
            TestLog.send("Message.onCancelButtonClick");
            contentText.text = previousText;
            close();   
        }
        protected static function onCancelButtonRemoved(event:Event):void {
            TestLog.send("Message.onCancelButtonRemoved"); 
            cancelButton.removeEventListener(MouseEvent.CLICK, onCancelButtonClick);
            cancelButton.removeEventListener(Event.REMOVED_FROM_STAGE, onCancelButtonRemoved);
        }

        public static function show(stage:Stage, callbackId:int, title:String, defaultStr : String)
        { 
            if (s_isOpen) { return; }
            s_stage = stage;
            
            // Close existing (if any).
            close();
      
            s_isOpen = true;
            m_callbackId = callbackId;
            previousText = defaultStr;

            // Create dialog. They overlay sprite overlays the entire scene.
            overlay  = new ModalCoverSprite();
            overlay.width = stage.stageWidth / 2;
            overlay.height = 150;
      
      
            // title
            titleText = new TextField();
            titleText.appendText(title);
            titleText.selectable = false;
            titleText.multiline = false;
            titleText.x = 10;
            titleText.y = 10;
            titleText.width = (stage.stageWidth/2) - 10;
            titleText.height = 30;
            titleText.border = false;
      
      
            var titleFormat:TextFormat = new TextFormat();
            titleFormat.font = "Arial";
            titleFormat.bold = true;
            titleFormat.color = 0xFFFFFF;
            titleFormat.size = 21;
            titleFormat.align = 'left';
            titleText.setTextFormat(titleFormat);

            overlay.addChild(titleText);
      
            // content area
            contentText = new TextField();
            contentText.appendText(defaultStr);
            contentText.selectable = true; 
            contentText.border = true; 
            contentText.multiline = false;
            contentText.x = 0;
            contentText.y = 65;
            contentText.width = (stage.stageWidth / 2);
            contentText.height = 60;
            contentText.border = false;
            contentText.type = "input";
      
            var contentFormat:TextFormat = new TextFormat();
            contentFormat.font = "Arial";
            contentFormat.bold = false;
            contentFormat.color = 0xFFFFFF;
            contentFormat.size = 18;
            contentFormat.align = 'center';
            contentText.setTextFormat(contentFormat);
            
            overlay.addChild(contentText);
 
            // ok button
            okButton = new TextField();
            okButton.appendText("OK");
            okButton.x = 2;
            okButton.y = 112
            okButton.width = (stage.stageWidth/4) - 4;  
            okButton.height = 30;
            okButton.selectable = false;
            okButton.backgroundColor = 0x000000FF;
            okButton.borderColor = 0xffffffFF;
            okButton.opaqueBackground = 0x000000;
      
            var okButtonFormat:TextFormat = new TextFormat();
            okButtonFormat.font = "Arial";
            okButtonFormat.bold = false;
            okButtonFormat.color = 0xFFFFFF;
            okButtonFormat.size = 18;
            okButtonFormat.align = 'center';
            okButton.setTextFormat(okButtonFormat);
           
            okButton.addEventListener(MouseEvent.CLICK, onOkButtonClick);
            okButton.addEventListener(Event.REMOVED_FROM_STAGE, onOkButtonRemoved);  
            overlay.addChild(okButton);
      
            // cancel button
            cancelButton = new TextField();
            cancelButton.appendText("Cancel");
            cancelButton.x = (stage.stageWidth/4)+4;  
            cancelButton.y = 112;
            cancelButton.width = (stage.stageWidth/4) - 4;  
            cancelButton.height = 30;
            cancelButton.selectable = false;
            cancelButton.backgroundColor = 0x000000FF;
            cancelButton.borderColor = 0xffffffFF;
            cancelButton.opaqueBackground = 0x000000;
      
            var cancelButtonFormat:TextFormat = new TextFormat();
            cancelButtonFormat.font = "Arial";
            cancelButtonFormat.bold = false;
            cancelButtonFormat.color = 0xFFFFFF;
            cancelButtonFormat.size = 18;
            cancelButtonFormat.align = 'center';
            cancelButton.setTextFormat(cancelButtonFormat);
           
            cancelButton.addEventListener(MouseEvent.CLICK, onCancelButtonClick);
            cancelButton.addEventListener(Event.REMOVED_FROM_STAGE, onCancelButtonRemoved);  
            overlay.addChild(cancelButton); 

            stage.addChild(overlay);

            // steal focus
            previousFocus =  stage.focus ;
            stage.focus = overlay;

            // Make sure the ugly yellow focus rectangle is hidden. Hiding it seems
            // to have one additional benefit. If we don’t do it, one can still 
            // change the focus using left and right arrow keys. The focus rectangle
            // actually moves to another element in the scene (first time I saw
            // something like this in Flash). If we hide it, the left and right arrow
            // keys actually don’t do anything. I did not find any documentation
            // related to this. So it may change in future. But if it changes,
            // possible workaround could be to simply catch the keyboard events
            // and prevent propagation. Such approach is also used in the official
            // AS3 documentation of the FocusEvent class:
            // 
            //   http://livedocs.adobe.com/
            //       flash/9.0/ActionScriptLangRefV3/flash/events/FocusEvent.html
            //
            stage.stageFocusRect = false;

            // Even though the message overlays the entire stage, if you click
            // on it, it loses focus. My guess is that Flash does not find
            // anything obvious which can be focused (such as a textbox). The
            // following below forces Flash to keep the focus on the message.
            // This approach is also used in an example in the AS3 documentation,
            // of the of the FocusEvent class (see the link above) so it gives it
            // some credibility.
            /*overlay.addEventListener(MouseEvent.CLICK,
                function(event : MouseEvent) : void { 
                 TestLog.send("Message.overlay MouseEvent.CLICK");

                  //stage.focus = overlay; 
                  //close();
                } 
            );  */
      
            stage.addEventListener(MouseEvent.CLICK,
                function(event : MouseEvent) : void { 
                    //event.stopPropagation(); 
                    
                    //TestLog.send("Message.overlay Event.ACTIVATE");
                    //TestLog.send(event.relatedObject);
                    //if (event.relatedObject != overlay ) { 
                    //  stage.focus = overlay;   
                    //}
                  
                    if (stage.focus == previousFocus) {
                      stage.focus = overlay;
                    }
                 // close();
                }
            );

            // In addition, when the entire Flash loses focus and it gets it later 
            // back, the container (browser) does not restore the previously focused
            // element. So we have to do it ourselves.
            stage.addEventListener(Event.ACTIVATE,
                function(event : Event) : void { 
                 TestLog.send("Message.overlay Event.ACTIVATE");

                  stage.focus = overlay;  
                 // close();
                }
            );

            // And apparently, swallow all keyboard events.
            stage.addEventListener(KeyboardEvent.KEY_DOWN,
                function(event : KeyboardEvent) : void { 
                TestLog.send("Message.overlay KeyboardEvent.KEY_DOWN");
                  event.stopPropagation(); 
    
                 // close();
                }, 1
            );

        }

        public static function close() : void
        { 
           TestLog.send("Message.close");

            if (s_stage != null &&  overlay != null) {            // snip ...
              //overlay.removeChild(dialog);
              //overlay.removeChild(titleText);
              //overlay.removeChild(contentText);
              
              while (overlay.numChildren > 0) {
                TestLog.send("removing child...");
                overlay.removeChildAt(0); 
              }
              
              s_stage.removeChild(overlay);
              s_stage.focus = previousFocus;
              previousFocus = null;
              s_stage = null;
              overlay = null;
              s_isOpen = false;

              //if (Console.inited) { 
                TestLog.send("calling callback function. ");
                var args:Vector.<int> = new Vector.<int>();
                args.push(m_callbackId);   
                CModule.callI(Console.s_console.m_nativeCallbackPtr, args);
             // } 
            }
        } 

     


    }




  public class SoundContainer 
  {
    private var m_sound:Sound;
    private var m_channel:SoundChannel;
    private var m_transform;
    private var m_isSoundPlaying:Boolean;
    private var m_pausePosition:int;

    public function SoundContainer(s:String, vfs:gamevfs)
    {
      TestLog.send("Setting SC vars to null");
      m_sound = null;
      m_channel = null;
      m_transform = null;
      m_isSoundPlaying = false;
      m_pausePosition = 0;

      TestLog.send("new sound");
      m_sound = new Sound(); //new URLRequest(s)); 
  
      TestLog.send("get bytearray from vfs");
      //var d:ByteArray = Console.s_console.s_gamevfs.getFile(s);
      //var d:ByteArray = Console.s_gamevfs.getFile(s);
      var d:ByteArray = vfs.getFile(s);

      TestLog.send("bytearray length: " + d.length);

      TestLog.send("load bytearray in to sound obj");
      m_sound.loadCompressedDataFromByteArray( d, d.length );

      TestLog.send("create new sound transform obj");
      m_transform = new SoundTransform();
      m_transform.volume = 1.0;

    }

    public function play():void
    {
      if(!m_isSoundPlaying) 
      {
        m_channel = m_sound.play(m_pausePosition);
        m_channel.addEventListener(Event.SOUND_COMPLETE, s_handleSoundComplete);
        m_channel.soundTransform = m_transform;
        
        m_isSoundPlaying = true;
      }
      else 
      {
        stop();
        m_pausePosition = 0;
        play();
      }
    }
    public function pause():void
    {
      m_pausePosition = m_channel.position; 
      m_channel.stop(); 
    }

    public function stop():void
    {
      if (m_isSoundPlaying)
      {
        m_channel.stop();
        m_isSoundPlaying = false;
      }
    }
    public function isPlaying():Boolean
    {
      return m_isSoundPlaying;
    }
    
    public function setVolume(v:Number):void
    {
      if (m_channel != null) {
        if (m_channel.soundTransform != null) {
          m_transform.volume = v;
          m_channel.soundTransform = m_transform;
        }
      }
    }

    private function s_handleSoundComplete(ev:Event):void
    {
      m_isSoundPlaying = false;
    }
  }

  /**
  * A basic implementation of a console for flascc apps.
  * The PlayerKernel class delegates to this for things like read/write
  * so that console output can be displayed in a TextField on the Stage.
  */
  public class Console extends Sprite implements ISpecialFile
  {
    private var vgl_mx:int, vgl_my:int, kp:int, vgl_buttons:int;
    
    // c++ function pointers;
    private var mainloopTickPtr:int, soundUpdatePtr:int, audioBufferPtr:int;
    private var m_mouseDownPtr:int
    private var m_mouseMovedPtr:int;
    private var m_mouseUpPtr:int;
    private var m_keyDownPtr:int;
    private var m_keyUpPtr:int;
    public var m_nativeCallbackPtr:int;
   
    


    private var inputContainer
    private var keybytes:ByteArray = new ByteArray()
    private var mx:int = 0, my:int = 0, last_mx:int = 0, last_my:int = 0, button:int = 0;
    private var snd:Sound = null
    private var sndChan:SoundChannel = null
    private var s3d:Stage3D;
    private var ctx3d:Context3D;
    private var rendered:Boolean = false;
    private var inited:Boolean = false
    private const emptyVec:Vector.<int> = new Vector.<int>();

    public var s_gamevfs:gamevfs = null;

    public var s_audioMap:Object = new Object();

   

    // Audio Impl
    public function loadSound(s:String):void
    {
        TestLog.send("AS3: loading sound " + s);
        //TestLog.send("as3 loading sound");

        var __s_gamevfs:gamevfs = new gamevfs();
        CModule.vfs.addBackingStore(__s_gamevfs, null);
        //CModule.vfs.addDirectory("/local");
        //CModule.vfs.addBackingStore(new LSOBackingStore("gamevfsLSO"), "/local");

        TestLog.send("sound container nonsense");
        var sound:SoundContainer = new SoundContainer(s, __s_gamevfs);
        s_audioMap[s] = sound;
        TestLog.send("sound container nonsense done");
        TestLog.sendObject(s_audioMap);
    }
    public function playSound(s:String):void
    {
        if (s_audioMap[s] == null) { 
        //    TestLog.send("AS3: audioMap[" + s + "] was null?");
        //    TestLog.send("AS3: " + s_audioMap.toString());
            return; 
        }

        TestLog.send("AS3: playing sound " + s);
        s_audioMap[s].play();
    }
    public static function pauseSound(s:String):void
    {
        TestLog.send("AS3: pausing sound " + s);
        Console.s_console.s_audioMap[s].pause();
    }
    public static function stopSound(s:String):void
    {
        TestLog.send("AS3: stopping sound " + s);
        Console.s_console.s_audioMap[s].stop();
    }
    public function isPlayingSound(s:String):int
    {
        if (s_audioMap[s] == null) { 
            TestLog.send("AS3: audioMap[" + s + "] was null?");
            TestLog.sendObject(s_audioMap);
            return 0; 
        }

        TestLog.send("AS3: is playing sound " + s);
        return (s_audioMap[s].isPlaying())?1:0;
    }
    public static function setSoundVolume(s:String, v:Number):void
    {
      //trace("AS3: setting sound volume " + s + " = " + v);
      Console.s_console.s_audioMap[s].setVolume(v);
    }
    
    // misc functions
    //public static var s_urlRequestDone:Boolean = false;
    public static var s_urlResponse:String = "";
    public static function urlRequest(s:String):void
    {
      var req:URLRequest = new URLRequest(s);
      //req.method = URLRequestMethod.POST;

      var loader:URLLoader = new URLLoader();
      loader.dataFormat = URLLoaderDataFormat.TEXT;
      loader.addEventListener(Event.OPEN, urlLoader_openHandler);
      loader.addEventListener(Event.COMPLETE, urlLoader_complete);
      loader.addEventListener(IOErrorEvent.IO_ERROR, urlLoader_ioErrorHandler);
      loader.addEventListener(SecurityErrorEvent.SECURITY_ERROR, urlLoader_securityErrorHandler);
      loader.load(req);

      function urlLoader_openHandler(evt:Event):void { 
        TestLog.send("url opened."); 
      }
      function urlLoader_ioErrorHandler(evt:Event):void { 
        //Console.s_urlRequestDone = true; 
        s_urlResponse = "io error";
        TestLog.send("io error"); 
      }
      function urlLoader_securityErrorHandler(evt:Event):void { 
        //Console.s_urlRequestDone = true; 
        s_urlResponse = "security error";
        TestLog.send("security error"); 
      }

      function urlLoader_complete(evt:Event):void { 
        //Console.s_urlRequestDone = true; 
        s_urlResponse = loader.data;
      }
      //Console.s_urlRequestDone = false;

     // var r:String = loader.data;
     // return r;
    }

    public static function goToUrl(urlstr:String):void {
        //var req = new URLRequest(urlstr);
        //navigateToURL(req, "_blank");  
    }

    public function openInputDialog(callbackId:int, ttitle:String, defaultStr:String)
    {
      TestLog.send("Console.openInputDialog()");  
 
      TestLog.send("callbackid: " + callbackId);
      TestLog.send("title: " + ttitle); 
      TestLog.send("defaultstr: " + defaultStr);
 
      //Alert.show("Please re-enter your Information.", "Your Username/Password was incorrect", Alert.OK, _parent, wrongpassword);
      //Alert.show(ttitle, ttitle, Alert.OK, Console.s_console.stage, defaultStr);    
      //Message.show(Console.s_console.stage, "Not implemented?");

      Message.show(Console.s_console.stage, callbackId, ttitle, defaultStr); 

     
    }  

    public function getInputDialogText():String {
      return Message.getText();
    }

    // fullscreen
    public static function setFullscreen(b:Boolean):void
    { 
      try
      {
        if (b) { 
          Console.s_console.stage.displayState = StageDisplayState.FULL_SCREEN; 
          //Console.s_console.systemManager.stage.displayState = StageDisplayState.FULL_SCREEN;
        } else {
          Console.s_console.stage.displayState = StageDisplayState.NORMAL; 
          //Console.s_console.systemManager.stage.displayState = StageDisplayState.NORMAL;
        }
      }
      catch(e:Error)
      {
        TestLog.send("" + e);
      }
    }

    public static var s_console:Console = null;
    /**
    * To Support the preloader case you might want to have the Console
    * act as a child of some other DisplayObjectContainer.
    */
    public function Console(container:DisplayObjectContainer = null)
    {
      s_console = this;

      CModule.rootSprite = container ? container.root : this

      if(CModule.runningAsWorker()) {
        return;
      }

      if(container) {
        container.addChild(this)
        init(null)
      } else {
        addEventListener(Event.ADDED_TO_STAGE, init)
      }
    }

    /**
    * All of the real FlasCC init happens in this method
    * which is either run on startup or once the SWF has
    * been added to the stage.
    */
    protected function init(e:Event):void
    {
      inputContainer = new Sprite()
      addChild(inputContainer)

      stage.frameRate = 60;
      stage.align = StageAlign.TOP_LEFT;
      stage.scaleMode = StageScaleMode.NO_SCALE;
      
      stage.addEventListener(KeyboardEvent.KEY_DOWN, bufferKeyDown);
      stage.addEventListener(KeyboardEvent.KEY_UP, bufferKeyUp);
      stage.addEventListener(MouseEvent.MOUSE_MOVE, bufferMouseMove);
      stage.addEventListener(MouseEvent.MOUSE_DOWN, bufferMouseDown);
      stage.addEventListener(MouseEvent.MOUSE_UP, bufferMouseUp);

      try {
        stage.addEventListener(MouseEvent.RIGHT_CLICK, rightClick);
      } catch(e:*) {
        // disable the right-click menu if possible
      }
      
      s3d = stage.stage3Ds[0];
      s3d.addEventListener(Event.CONTEXT3D_CREATE, context_created);
      /*
      try {
        // If we're in FP 11.4+ this should request a constrained Stage3D context
        s3d.requestContext3D(Context3DRenderMode.AUTO, Context3DProfile.BASELINE_CONSTRAINED)
      } catch(e:*) {
        // If that failed we're in an older FP 11 player so we try for a normal Stage3D context
        s3d.requestContext3D(Context3DRenderMode.AUTO)
      }
      */
      s3d.requestContext3D(Context3DRenderMode.AUTO)

     
    }

    private function rightClick(e:Event):void
    {
      // no legacy right click menu! yay!
    }
    
    protected var vertices:Vector.<Number>;
    protected var vertexbuffer:VertexBuffer3D;
    protected var indexbuffer:IndexBuffer3D; 
    protected var program:Program3D;

    private function context_created(e:Event):void
    {
      ctx3d = s3d.context3D
      ctx3d.configureBackBuffer(stage.stageWidth, stage.stageHeight, 0, true /*enableDepthAndStencil*/ )
      ctx3d.enableErrorChecking = false;
      trace("Stage3D context: " + ctx3d.driverInfo);

      if(ctx3d.driverInfo.indexOf("Software") != -1) {
          trace("Software mode unsupported...");
          return;
      }
      
      GLAPI.init(ctx3d, null, stage);
      var gl:GLAPI = GLAPI.instance;
      //gl.log = new TestLog(); 
      gl.context.clear(0.0, 0.0, 0.0);
      gl.context.present();


      vertices = Vector.<Number>([
      -0.3,-0.3,0, 1, 0, 0, // x, y, z, r, g, b
      -0.3, 0.3, 0, 0, 1, 0,
      0.3, 0.3, 0, 0, 0, 1]);

      // Create VertexBuffer3D. 3 vertices, of 6 Numbers each
      vertexbuffer = GLAPI.instance.context.createVertexBuffer(3, 6);
      // Upload VertexBuffer3D to GPU. Offset 0, 3 vertices
      vertexbuffer.uploadFromVector(vertices, 0, 3);  

      var indices:Vector.<uint> = Vector.<uint>([0, 1, 2]);
      // Create IndexBuffer3D. Total of 3 indices. 1 triangle of 3 vertices
      indexbuffer = GLAPI.instance.context.createIndexBuffer(3);     
      // Upload IndexBuffer3D to GPU. Offset 0, count 3
      indexbuffer.uploadFromVector (indices, 0, 3); 

      var vertexShaderAssembler : AGALMiniAssembler = new AGALMiniAssembler();
      vertexShaderAssembler.assemble( Context3DProgramType.VERTEX,
        "m44 op, va0, vc0\n" + // pos to clipspace
        "mov v0, va1" // copy color
      );      

      var fragmentShaderAssembler : AGALMiniAssembler= new AGALMiniAssembler();
      fragmentShaderAssembler.assemble( Context3DProgramType.FRAGMENT,
        "mov oc, v0"
      );

      program = GLAPI.instance.context.createProgram();
      program.upload( vertexShaderAssembler.agalcode, fragmentShaderAssembler.agalcode);

      this.addEventListener(Event.ENTER_FRAME, enterFrame);
      stage.addEventListener(Event.RESIZE, stageResize);

    
    }
    
    private function stageResize(event:Event):void
    { 
        // need to reconfigure back buffer
        //ctx3d.configureBackBuffer(stage.stageWidth, stage.stageHeight, 2, true /*enableDepthAndStencil*/ )
        ctx3d.configureBackBuffer(stage.stageWidth, stage.stageHeight, 0, true /*enableDepthAndStencil*/ )
    }

    /**
    * The callback to call when FlasCC code calls the <code>posix exit()</code> function. Leave null to exit silently.
    * @private
    */
    public var exitHook:Function;

    /**
    * The PlayerKernel implementation will use this function to handle
    * C process exit requests
    */
    public function exit(code:int):Boolean
    {
      // default to unhandled
      return exitHook ? exitHook(code) : false;
    }

    /**
    * The PlayerKernel implementation uses this function to handle
    * C IO write requests to the file "/dev/tty" (for example, output from
    * printf will pass through this function). See the ISpecialFile
    * documentation for more information about the arguments and return value.
    */
    public function write(fd:int, bufPtr:int, nbyte:int, errnoPtr:int):int
    {
      var str:String = CModule.readString(bufPtr, nbyte)
      consoleWrite(str)
      return nbyte
    }

    /**
    * libVGL expects to be able to read Keyboard input from
    * file descriptor zero using normal C IO.
    */
    public function read(fd:int, bufPtr:int, nbyte:int, errnoPtr:int):int
    {
      if(fd == 0 && nbyte == 1) {
        keybytes.position = kp++
        if(keybytes.bytesAvailable) {
          CModule.write8(bufPtr, keybytes.readUnsignedByte())
        } else {
        keybytes.position = 0
        kp = 0
        } 
      }
      return 0
    }

    /**
    * The PlayerKernel implementation uses this function to handle
    * C fcntl requests to the file "/dev/tty." 
    * See the ISpecialFile documentation for more information about the
    * arguments and return value.
    */
    public function fcntl(fd:int, com:int, data:int, errnoPtr:int):int
    {
      return 0
    }

    /**
    * The PlayerKernel implementation uses this function to handle
    * C ioctl requests to the file "/dev/tty." 
    * See the ISpecialFile documentation for more information about the
    * arguments and return value.
    */
    public function ioctl(fd:int, com:int, data:int, errnoPtr:int):int
    {
      vglttyargs[0] = fd
      vglttyargs[1] = com
      vglttyargs[2] = data
      vglttyargs[3] = errnoPtr
      return CModule.callI(CModule.getPublicSymbol("vglttyioctl"), vglttyargs);
    }
    private var vglttyargs:Vector.<int> = new Vector.<int>()
    
    /**
    * Helper function that traces to the flashlog text file and also
    * displays output in the on-screen textfield console.
    */
    protected function consoleWrite(s:String):void
    {
      TestLog.send(s)
    }

    public function bufferMouseMove(me:MouseEvent) {
      me.stopPropagation();
      mx = me.stageX;
      my = me.stageY;

      if (Message.isOpen()) { 
        TestLog.send("bufferMouseMove message.isOpen() true");
        return; 
      }

      if (inited) {
        var args:Vector.<int> = new Vector.<int>();
        args.push(mx);
        args.push(my);
        CModule.callI(m_mouseMovedPtr, args);
      }
    }
/*
    :int
    private var :int;
    private var :int;*/
    
    public function bufferMouseDown(me:MouseEvent) 
    {
      me.stopPropagation();
      mx = me.stageX;
      my = me.stageY;
      button = 1;

      if (Message.isOpen()) { 
        TestLog.send("bufferMouseMove message.isOpen() true");
        return; 
      }

      if (inited) {
        var args:Vector.<int> = new Vector.<int>();
        args.push(0); // 0 for LMB, 1 for RMB.
        args.push(mx);
        args.push(my);
        CModule.callI(m_mouseDownPtr, args);
      }
    }
    
    public function bufferMouseUp(me:MouseEvent) 
    {
      me.stopPropagation();
      mx = me.stageX;
      my = me.stageY;
      button = 0;

      if (Message.isOpen()) { 
        TestLog.send("bufferMouseMove message.isOpen() true");
        return; 
      }

      if (inited) {
        var args:Vector.<int> = new Vector.<int>();
        args.push(0); // 0 for LMB, 1 for RMB.
        args.push(mx);
        args.push(my);
        CModule.callI(m_mouseUpPtr, args);
      }
    }

    public function bufferKeyDown(ke:KeyboardEvent) 
    {
      //if(Keyboard.capsLock || ke.keyCode >= 127)
      //  return;

      if (Message.isOpen()) { 
        TestLog.send("bufferMouseMove message.isOpen() true");
        return; 
      }

      //keybytes.writeByte(int(ke.keyCode & 0x7F));
      if (inited) {
        var args:Vector.<int> = new Vector.<int>();
        args.push(ke.keyCode);
        CModule.callI(m_keyDownPtr, args);
      }
   
    }
    
    public function bufferKeyUp(ke:KeyboardEvent) 
    {
      //if (Keyboard.capsLock || ke.keyCode > 128)
      //  return;

      if (Message.isOpen()) { 
        TestLog.send("bufferMouseMove message.isOpen() true");
        return; 
      }

      //keybytes.writeByte(int(ke.keyCode | 0x80));
       if (inited) {
        var args:Vector.<int> = new Vector.<int>();
        args.push(ke.keyCode); 
        CModule.callI(m_keyUpPtr, args);
      }
    }


    // ----------------------------------------------------------
    // ----------------------------------------------------------
    // ----------------------------------------------------------
    
    // GAMEPAD API

    // ----------------------------------------------------------
    // ----------------------------------------------------------
    // ----------------------------------------------------------
    //public static var m_gameInput:GameInput;
    
    public static function onGamepadAttached(gamepadIndex:int, gamepadName:String):void {
      trace("gamepad attached. id: " + gamepadIndex + ". name: " + gamepadName + ".");
    }
    public static function onGamepadButtonAttached(gamepadIndex:int, buttonId:int):void {
      trace("gamepad button attached. id: " + gamepadIndex + ". button: " + buttonId); 
    }
    public static function onGamepadAxisAttached(gamepadIndex:int, axisId:int, minValue:Number, maxValue:Number):void {
      trace("gamepad axis attached. id: " + gamepadIndex + ". axis: " + axisId + ". min: " + minValue + ". max: " + maxValue + "."); 
    }
    
    public static function onGamepadRemoved(gamepadIndex:int, gamepadName:String):void {
      trace("gamepad removed. id: " + gamepadIndex + ". name: " + gamepadName + ".");
    }
    
    public static function onGamepadButtonDown(gamepadId:int, buttonId:int):void {
      trace("gamepad button down. gamepad id: " + gamepadId + ". button: " + buttonId);
    }
    public static function onGamepadButtonUp(gamepadId:int, buttonId:int):void {
      trace("gamepad button up. gamepad id: " + gamepadId + ". button: " + buttonId);
    }
    public static function onGamepadAxisChanged(gamepadId:int, axisId:int, value:Number):void {
      trace("gamepad axis changed. gamepad id: " + gamepadId + ". axis: " + axisId + ". value: " + value);
    }
    
    /*
    
    We have gamepads working in Flash.
    Now to hook it up to CrossBridge / FlasCC to make a nice web version of #theagent #glorytomoscow. Huzzah!
    
    */
    
    
    //public function flashGamepadAttached(e:GameInputEvent):void {
      //trace("device is added: " + e.device.id);
      
     /* for (var k:Number = 0; k < GameInput.numDevices; k++)
      {
        var device:GameInputDevice = GameInput.getDeviceAt(k);
        if (device == e.device) 
        {
          onGamepadAttached(k, e.device.id); 
          
          var _controls:Vector.<String> = new Vector.<String>;
          device.enabled = true;

          for (var i:Number = 0; i < device.numControls; i++) 
          {
            var control:GameInputControl = device.getControlAt(i);
            control.addEventListener(Event.CHANGE, onChange);
            _controls[i] = control.id;
            
            if (isButton(control)) {
              onGamepadButtonAttached(k, getButtonId(control));
            } else if (isAxis(control)) {
              onGamepadAxisAttached(k, getAxisId(control), control.minValue, control.maxValue); 
            }
          } 

          device.startCachingSamples(30, _controls);
        }
      }*/

      //for(var j:int = 0; j < _controls.length; j++)
      //{
      //  trace("" + _controls[j]);
      //}
    //}
    //public function flashGamepadRemoved(e:GameInputEvent):void {
      /*for (var k:Number = 0; k < GameInput.numDevices; k++)
      {
        var device:GameInputDevice = GameInput.getDeviceAt(k);
        if (device == e.device) {
          onGamepadRemoved(k, e.device.id); 
        }
        
      }*/
      
    //}
    
    //public static function getDeviceIndex(device:GameInputDevice):int 
    //{
     /* for (var k:Number = 0; k < GameInput.numDevices; k++)
      {
        var otherDevice:GameInputDevice = GameInput.getDeviceAt(k);
        if (otherDevice == device) {
          return k;
        }
      }*/
      //trace("could not get index for device. weird?!");
      //return 0;
    //}
    
    //public static function isButton(control:GameInputControl):Boolean {
     /* if (control.id.indexOf("BUTTON_") != -1) {
        if (control.id.indexOf("BUTTON_10") != -1 || control.id.indexOf("BUTTON_11") != -1) { 
          return false;
        }
        return true;
      }*/
    //  return false;
    //}
    //public static function isAxis(control:GameInputControl):Boolean {
      /*if (control.id.indexOf("AXIS_") != -1) {
        return true;
      } else if (control.id.indexOf("BUTTON_10") != -1 || control.id.indexOf("BUTTON_11") != -1) { 
        return true; 
      }*/
    //  return false;
    //}
    
    protected static function onChange(event:Event):void 
    {
      /*var control:GameInputControl = event.target as GameInputControl;
  
      if (control.id.indexOf("BUTTON_") != -1) {
        
        // triggers. these are usually axises...?
        if (control.id.indexOf("BUTTON_10") != -1 || control.id.indexOf("BUTTON_11") != -1) { 
          onGamepadAxisChanged(getDeviceIndex(control.device), getAxisId(control), control.value);
          return;  
        }
        
        //trace("" + control.id);
        //trace("" + control.value);
        if (control.value == 1) { 
          //trace("" + control.id + " down");
          onGamepadButtonDown(getDeviceIndex(control.device), getButtonId(control));
        } else if (control.value == 0){
          //trace("" + control.id + " up");
          onGamepadButtonUp(getDeviceIndex(control.device), getButtonId(control));
        } else {
          trace("" + control.id + " value " + control.value);
        }
      }
      
      // 0: L stick x
      // 1: L stick y
      // 2: R stick x
      // 3: R stick y
      else if (control.id.indexOf("AXIS_") != -1) {
        //trace("" + control.id);
        //trace("" + control.value);
          onGamepadAxisChanged(getDeviceIndex(control.device), getAxisId(control), control.value);
      }
      */
    }
    
    
    public static const BUTTON_A:int = 0;
    public static const BUTTON_B:int = 1;
    public static const BUTTON_X:int = 2;
    public static const BUTTON_Y:int = 3;
    public static const BUTTON_LBUMPER:int = 4;
    public static const BUTTON_RBUMPER:int = 5;
    public static const BUTTON_BACK:int = 6;
    public static const BUTTON_START:int = 7;
    public static const BUTTON_L3:int = 8; 
    public static const BUTTON_R3:int = 9;
    public static const BUTTON_ACTIVATE:int = 10;

    public static const DPAD_LEFT:int = 101;
    public static const DPAD_UP:int = 103;
    public static const DPAD_RIGHT:int = 105;
    public static const DPAD_DOWN:int = 107;
    
        
    //public static function getButtonId(control:GameInputControl):int {
      /*if (control.id == "BUTTON_4") {
        return BUTTON_A;
      } else if (control.id == "BUTTON_5") {
        return BUTTON_B;
      } else if (control.id == "BUTTON_6") {
        return BUTTON_X;
      } else if (control.id == "BUTTON_7") {
        return BUTTON_Y;
      } else if (control.id == "BUTTON_8") {
        return BUTTON_LBUMPER;
      } else if (control.id == "BUTTON_9") {
        return BUTTON_RBUMPER;
      } else if (control.id == "BUTTON_12") {
        return BUTTON_BACK;
      } else if (control.id == "BUTTON_13") {
        return BUTTON_START;
      } else if (control.id == "BUTTON_14") {
        return BUTTON_L3;
      } else if (control.id == "BUTTON_15") {
        return BUTTON_R3;
      } else if (control.id == "BUTTON_16") {
        return DPAD_UP;
      } else if (control.id == "BUTTON_17") {
        return DPAD_DOWN;
      } else if (control.id == "BUTTON_18") {
        return DPAD_LEFT;
      } else if (control.id == "BUTTON_19") {
        return DPAD_RIGHT;
      }
      trace("invalid button id: " + control.id);*/
    //  return BUTTON_A;
    //}
    
    public static const ANALOG_STICK_1_X:int = 0;
    public static const ANALOG_STICK_1_Y:int = 1;
    public static const ANALOG_STICK_2_X:int = 2;
    public static const ANALOG_STICK_2_Y:int = 3;
    public static const TRIGGER_1:int = 4;
    public static const TRIGGER_2:int = 5;

        
    //public static function getAxisId(control:GameInputControl):int {
      /*if (control.id == "AXIS_0") {
        return ANALOG_STICK_1_X;
      } else if (control.id == "AXIS_1") {
        return ANALOG_STICK_1_Y;
      } else if (control.id == "AXIS_2") {
        return ANALOG_STICK_2_X;
      } else if (control.id == "AXIS_3") {
        return ANALOG_STICK_2_Y;
      }  else if (control.id == "BUTTON_10") {
        return TRIGGER_1;
      } else if (control.id == "BUTTON_11") {
        return TRIGGER_2;
      }*/
    //  trace("invalid axis id");
    //  return ANALOG_STICK_1_X;
    //}
    
    
          
    
    
    public function flashVersionName():void {
      // get & split information arrays
      var _fullInfo:String = Capabilities.version;
      var _osSplitArr:Array = _fullInfo.split(' ');
      var _versionSplitArr:Array = _osSplitArr[1].split(',');
       
      // declare the relevant infos
      var _osInfo:String = _osSplitArr[0];
      var _versionInfo:Number = _versionSplitArr[0];
      var _versionInfo2:Number = _versionSplitArr[1];
       
      // quick check
      trace("major version you're using: " + _versionInfo);
      trace("minor version you're using: " + _versionInfo2);
      trace("you're using the following OS: " + _osInfo);
    }
    // ----------------------------------------------------------
    // ----------------------------------------------------------
    // ----------------------------------------------------------




    protected function gameinit():void
    {
      if (!inited) {
        inited = true 
        
        s_gamevfs = new gamevfs();
        CModule.vfs.console = this;
        CModule.vfs.addBackingStore(s_gamevfs, null);
        CModule.vfs.addDirectory("/local");
        CModule.vfs.addBackingStore(new LSOBackingStore("gamevfsLSO"), "/local");

        CModule.startAsync(this, new <String>["/game.swf"])

        vgl_mx = CModule.getPublicSymbol("vgl_cur_mx")
        vgl_my = CModule.getPublicSymbol("vgl_cur_my")
        vgl_buttons = CModule.getPublicSymbol("vgl_cur_buttons")
        //mainloopTickPtr = CModule.getPublicSymbol("mainLoopTick")
        //mainloopTickPtr = CModule.getPublicSymbol("_Z12flascc_tickv");
        mainloopTickPtr = CModule.getPublicSymbol("_Z11flascc_tickv");

        //threadedLogPtr = CModule.getPublicSymbol("_Z11flascc_logv");

        m_mouseDownPtr = CModule.getPublicSymbol("_Z22flascc_event_mouseDowniii");
        m_mouseMovedPtr = CModule.getPublicSymbol("_Z23flascc_event_mouseMovedii");
        m_mouseUpPtr = CModule.getPublicSymbol("_Z20flascc_event_mouseUpiii");
 
        m_keyDownPtr = CModule.getPublicSymbol("_Z20flascc_event_keyDowni"); 
        m_keyUpPtr = CModule.getPublicSymbol("_Z18flascc_event_keyUpi");

        m_nativeCallbackPtr = CModule.getPublicSymbol("_Z25flascc_nativeCallbackByIdi");

        /*m_gameInput = new GameInput();
        m_gameInput.addEventListener(GameInputEvent.DEVICE_ADDED , flashGamepadAttached);
        m_gameInput.addEventListener(GameInputEvent.DEVICE_REMOVED, flashGamepadRemoved);
        */

      }
    }

    /**
    * The enterFrame callback is run once every frame. UI thunk requests should be handled
    * here by calling <code>CModule.serviceUIRequests()</code> (see CModule ASdocs for more information on the UI thunking functionality).
    */
    protected function enterFrame(e:Event):void 
    {
      gameinit();

 

      CModule.serviceUIRequests()
      //CModule.write32(vgl_mx, mx);
      //CModule.write32(vgl_my, my); 
      //CModule.write32(vgl_buttons, button);

      GLAPI.instance.context.clear ( 0.5, 0.5, 0.5, 1 );


      CModule.callI(mainloopTickPtr, emptyVec);

      //CModule.callI(mainloopTickPtr, emptyVec);
 
    /*
      // vertex position to attribute register 0
      GLAPI.instance.context.setVertexBufferAt (0, vertexbuffer, 0,  Context3DVertexBufferFormat.FLOAT_3);
      // color to attribute register 1
      GLAPI.instance.context.setVertexBufferAt(1, vertexbuffer, 3,   Context3DVertexBufferFormat.FLOAT_3);
      // assign shader program
      GLAPI.instance.context.setProgram(program);

      var m:Matrix3D = new Matrix3D();
      m.appendRotation(getTimer()/40, Vector3D.Z_AXIS);
      GLAPI.instance.context.setProgramConstantsFromMatrix(Context3DProgramType.VERTEX, 0, m, true);

      //GLAPI.instance.context.drawTriangles(indexbuffer);
    */
     

      GLAPI.instance.context.present();
    }
  }
}
