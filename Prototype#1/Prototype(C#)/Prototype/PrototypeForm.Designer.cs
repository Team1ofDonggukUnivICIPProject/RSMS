namespace Prototype
{
    partial class PrototypeForm
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.picb_original = new OpenCvSharp.UserInterface.PictureBoxIpl();
            this.picb_result = new OpenCvSharp.UserInterface.PictureBoxIpl();
            this.lbl_motionnotifier = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.picb_original)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.picb_result)).BeginInit();
            this.SuspendLayout();
            // 
            // picb_original
            // 
            this.picb_original.Location = new System.Drawing.Point(52, 105);
            this.picb_original.Name = "picb_original";
            this.picb_original.Size = new System.Drawing.Size(611, 457);
            this.picb_original.TabIndex = 0;
            this.picb_original.TabStop = false;
            // 
            // picb_result
            // 
            this.picb_result.Location = new System.Drawing.Point(700, 105);
            this.picb_result.Name = "picb_result";
            this.picb_result.Size = new System.Drawing.Size(611, 457);
            this.picb_result.TabIndex = 1;
            this.picb_result.TabStop = false;
            // 
            // lbl_motionnotifier
            // 
            this.lbl_motionnotifier.AutoSize = true;
            this.lbl_motionnotifier.Font = new System.Drawing.Font("맑은 고딕", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.lbl_motionnotifier.ForeColor = System.Drawing.Color.Lime;
            this.lbl_motionnotifier.Location = new System.Drawing.Point(54, 44);
            this.lbl_motionnotifier.Name = "lbl_motionnotifier";
            this.lbl_motionnotifier.Size = new System.Drawing.Size(0, 41);
            this.lbl_motionnotifier.TabIndex = 2;
            this.lbl_motionnotifier.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // PrototypeForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1354, 605);
            this.Controls.Add(this.lbl_motionnotifier);
            this.Controls.Add(this.picb_result);
            this.Controls.Add(this.picb_original);
            this.Name = "PrototypeForm";
            this.Style = MetroFramework.MetroColorStyle.Silver;
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.OnDestroy);
            this.Load += new System.EventHandler(this.OnLoad);
            ((System.ComponentModel.ISupportInitialize)(this.picb_original)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.picb_result)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private OpenCvSharp.UserInterface.PictureBoxIpl picb_original;
        private OpenCvSharp.UserInterface.PictureBoxIpl picb_result;
        private System.Windows.Forms.Label lbl_motionnotifier;
    }
}

