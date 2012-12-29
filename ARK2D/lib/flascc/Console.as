package com.adobe.flascc
{
  import flash.display.Bitmap;
  import flash.display.BitmapData;
  import flash.display.DisplayObjectContainer; 
  import flash.display.Sprite;
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
  import flash.trace.Trace;
  import flash.ui.Keyboard;
  import flash.utils.ByteArray;
  import flash.utils.Endian;
  import flash.utils.getTimer;
  import flash.utils.*;

  import com.adobe.utils.*;
  import com.adobe.utils.macro.*;
  import com.adobe.flascc.CModule;
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
        public function send(value:String):void
        {
            trace("LOG: " + value);
        }
    }


  public class SoundContainer 
  {
    private var m_sound:Sound;
    private var m_channel:SoundChannel;
    private var m_transform;
    private var m_isSoundPlaying:Boolean;
    private var m_pausePosition:int;

    public function SoundContainer(s:String)
    {
      m_sound = null;
      m_channel = null;
      m_transform = null;
      m_isSoundPlaying = false;
      m_pausePosition = 0;

      m_sound = new Sound(); //new URLRequest(s)); 
  
      var d:ByteArray = Console.s_gamevfs.getFile(s);
      m_sound.loadCompressedDataFromByteArray( d, d.length );
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

    public static var s_gamevfs:gamevfs = null;

    public static var s_audioMap:Object = new Object();
    // Audio Impl
    public static function loadSound(s:String):void
    {
        trace("AS3: loading sound " + s);
        
        var sound:SoundContainer = new SoundContainer(s);
        s_audioMap[s] = sound;
    }
    public static function playSound(s:String):void
    {
        trace("AS3: playing sound " + s);
        s_audioMap[s].play();
    }
    public static function pauseSound(s:String):void
    {
        trace("AS3: pausing sound " + s);
        s_audioMap[s].pause();
    }
    public static function stopSound(s:String):void
    {
        trace("AS3: stopping sound " + s);
        s_audioMap[s].stop();
    }
    public static function isPlayingSound(s:String):int
    {
        trace("AS3: is playing sound " + s);
        return (s_audioMap[s].isPlaying())?1:0;
    }
    public static function setSoundVolume(s:String, v:Number):void
    {
      trace("AS3: setting sound volume " + s + " = " + v);
      s_audioMap[s].setVolume(v);
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
        trace("url opened."); 
      }
      function urlLoader_ioErrorHandler(evt:Event):void { 
        //Console.s_urlRequestDone = true; 
        s_urlResponse = "io error";
        trace("io error"); 
      }
      function urlLoader_securityErrorHandler(evt:Event):void { 
        //Console.s_urlRequestDone = true; 
        s_urlResponse = "security error";
        trace("security error"); 
      }

      function urlLoader_complete(evt:Event):void { 
        //Console.s_urlRequestDone = true; 
        s_urlResponse = loader.data;
      }
      //Console.s_urlRequestDone = false;

     // var r:String = loader.data;
     // return r;
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
        trace(e);
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
        ctx3d.configureBackBuffer(stage.stageWidth, stage.stageHeight, 2, true /*enableDepthAndStencil*/ )
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
      trace(s)
    }

    public function bufferMouseMove(me:MouseEvent) {
      me.stopPropagation()
      mx = me.stageX
      my = me.stageY

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

      //keybytes.writeByte(int(ke.keyCode | 0x80));
       if (inited) {
        var args:Vector.<int> = new Vector.<int>();
        args.push(ke.keyCode); 
        CModule.callI(m_keyUpPtr, args);
      }
    }

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

        m_mouseDownPtr = CModule.getPublicSymbol("_Z22flascc_event_mouseDowniii");
        m_mouseMovedPtr = CModule.getPublicSymbol("_Z23flascc_event_mouseMovedii");
        m_mouseUpPtr = CModule.getPublicSymbol("_Z20flascc_event_mouseUpiii");
 
        m_keyDownPtr = CModule.getPublicSymbol("_Z20flascc_event_keyDowni");
        m_keyUpPtr = CModule.getPublicSymbol("_Z18flascc_event_keyUpi");

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
